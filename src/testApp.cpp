#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofBackground(0);
    center.set(ofGetWidth()/2,ofGetHeight()/2);
    SIZE = (ofGetWidth()/GRID_WIDTH*1.0f)*0.5;
	space      = sin((1/6.0f)*2*M_PI)*SIZE*2;
    float space2      = sin((1/6.0f)*2*M_PI)*SIZE*1.7;
	float minX = center.x - (GRID_WIDTH * space);
	float minY = center.y - (GRID_HEIGHT * space);
	float maxWidth = (GRID_WIDTH * space+((GRID_HEIGHT%2)*0.5)*space) + center.x - (GRID_WIDTH * space)*0.5 - minX;
	float maxHeight = (GRID_HEIGHT * space) + center.y - (GRID_HEIGHT * space)*0.5  - minY;
    
	for (int j=0; j<GRID_HEIGHT; j++) {
		for (int i=0; i<GRID_WIDTH; i++) {
			int x	  = (i * space+((j%2)*0.5)*space) + center.x - (GRID_WIDTH * space)*0.5;//
			int y     = -ofRandom(100);//(j * space2) + center.y - (GRID_HEIGHT * space2)*0.5;
			int z     = 0;
			int index = (j*GRID_WIDTH+i) * LENGTH;
            int index2 = (j*GRID_WIDTH+i);
			bStart[index2] = false;
			// now we are at each line
			int hex_index = 0;
			
			float rdnSize = 1;
			float hue = 1;
            //            float hue = ofRandom(1);
            center_pos[index2].set(x,y,z);
            acc[index2].set(0,ofRandom(0.1,2),0);
            
			for (int k=0; k<LENGTH; k++) {
                color[index + k] = ofFloatColor::fromHsb(1,0,1);
                
                pos[index +k].x = x+sin((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                pos[index +k].y = y+cos((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                //				tex_coord[index +k].x = ofMap(pos[index +k].x,minX , maxWidth , 0,640);
                
                //				tex_coord[index +k].y = ofMap(pos[index +k].y,minY , maxHeight, 0,480);
                hex_index++;
				pos[index +k].z = z;
                
                loc[index +k] = pos[index +k];
                
			}
            
			
			
		}
	}
	total = GRID_WIDTH*GRID_HEIGHT*LENGTH;
	vbo.setVertexData(pos, total, GL_DYNAMIC_DRAW);
    //	vbo.setColorData(color, total, GL_DYNAMIC_DRAW);
    //	vbo.setTexCoordData(tex_coord, total, GL_DYNAMIC_DRAW);
    
    //	glEnable(GL_DEPTH_TEST);
    ofSetupScreenOrtho();
    count = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    
    timeCount+=ofGetLastFrameTime();
    float t = (timeCount) * 0.8f;
    float div = 250.0;
 
    for (int j=0; j<GRID_HEIGHT; j++) {
        for (int i=0; i<GRID_WIDTH; i++) {
            
            int index = (j*GRID_WIDTH+i) * LENGTH;
            int index2 = (j*GRID_WIDTH+i);
            if(bStart[index2])
            {
                ofVec3f vec(ofSignedNoise(t, center_pos[index2].y/div, center_pos[index2].z/div),
                            acc[index2].y,
                            0);
                center_pos[index2]+=vec;
                float rdnSize = 1;
                if(center_pos[index2].y>ofGetHeight()+100)
                {
                    center_pos[index2].y = -100;
                    int hex_index = 0;
                    
                    
                    for (int k=0; k<LENGTH; k++) {
                        
                        loc[index +k].x = center_pos[index2].x+sin((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                        loc[index +k].y = center_pos[index2].y+cos((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                        loc[index +k].z = 0;
                        hex_index++;
                    }
                }
                for (int k=0; k<LENGTH; k++) {
                    loc[index +k].x = center_pos[index2].x+sin((k/6.0f)*2*M_PI)*SIZE*rdnSize;
                    loc[index +k].y = center_pos[index2].y+cos((k/6.0f)*2*M_PI)*SIZE*rdnSize;
                    
                    //                pos[index+k] = loc[index+k];
                    pos[index+k] = loc[index+k].getRotated(-vec.y-center_pos[index2].y,center_pos[index2], ofVec3f(1,0,1));
                    
                    
                }
            }
            
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix();
	ofEnableAlphaBlending();
    //	ofSetColor(255, 255, 255);
	
    
	vbo.bind();
	vbo.updateVertexData(pos, total);
    //	vbo.updateColorData(color, total);
    //	vbo.updateTexCoordData(tex_coord, total);
    
	for (int i=0; i<GRID_WIDTH; i++) {
		for (int j=0; j<GRID_HEIGHT; j++) {
			int index = (j*GRID_WIDTH+i) * LENGTH;
            vbo.draw(GL_TRIANGLE_FAN, index,LENGTH);
		}
	}
    
	vbo.unbind();
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20,20);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    for (int j=0; j<GRID_HEIGHT; j++) {
        for (int i=0; i<GRID_WIDTH; i++) {
            
            int index2 = (j*GRID_WIDTH+i);
            bStart[index2] = !bStart[index2];
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}