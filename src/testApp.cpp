#include "testApp.h"
void testApp::exit(){
//	if(saver.isRecording()) saver.finishMovie();
}
//--------------------------------------------------------------
void testApp::setup(){
    
//    saver.setup(ofGetWidth()   ,ofGetHeight(),30);
//    image.allocate(ofGetWidth()   ,ofGetHeight(), OF_IMAGE_COLOR_ALPHA);
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
            

			int index   =   (j*GRID_WIDTH+i) * LENGTH;
            int index2  =   (j*GRID_WIDTH+i);

            center_pos[index2].y = ofRandom(-10000,-800);
            center_pos[index2].z = ofRandom(-RANGE,RANGE);
            
            int x	  = (i * space+((j%2)*0.5)*space) + center.x - (GRID_WIDTH * space);//
			int y     = center_pos[index2].y;//(j * space2) + center.y - (GRID_HEIGHT * space2)*0.5;
			int z     = center_pos[index2].z;
            
			bStart[index2] = false;
			// now we are at each line
			int hex_index = 0;
			
			float rdnSize = 1;
			float hue = 1;
            //            float hue = ofRandom(1);
            center_pos[index2].set(x,y,z);
            acc[index2].set(0,ofRandom(1,2),0);
            
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
    startThread();
    
    light.setPosition(1000, 1000, 2000);

    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(true);
    post.createPass<DofPass>()->setEnabled(true);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<NoiseWarpPass>()->setEnabled(false);
    post.createPass<PixelatePass>()->setEnabled(false);
    post.createPass<EdgePass>()->setEnabled(false);
    
    server.setName("hexSnow");
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
            //                if(bStart[index2])
            {
                ofVec3f vec(ofSignedNoise(t, center_pos[index2].y/div, center_pos[index2].z/div),
                            acc[index2].y,
                            0);
                center_pos[index2]+=vec;
                float rdnSize = 1;
                if(center_pos[index2].y>ofGetHeight()+100)
                {
                    center_pos[index2].y = ofRandom(-1000,-500);
                    center_pos[index2].z = ofRandom(-RANGE,RANGE);
                    int hex_index = 0;
                    
                    
                    for (int k=0; k<LENGTH; k++) {
                        
                        loc[index +k].x = center_pos[index2].x+sin((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                        loc[index +k].y = center_pos[index2].y+cos((hex_index/6.0f)*2*M_PI)*SIZE*rdnSize;
                        loc[index +k].z = center_pos[index2].z;
                        hex_index++;
                    }
                }
                for (int k=0; k<LENGTH; k++) {
                    loc[index +k].x = center_pos[index2].x+sin((k/6.0f)*2*M_PI)*SIZE*rdnSize;
                    loc[index +k].y = center_pos[index2].y+cos((k/6.0f)*2*M_PI)*SIZE*rdnSize;
                    loc[index +k].z = center_pos[index2].z;
                    //                pos[index+k] = loc[index+k];
                    pos[index+k] = loc[index+k].getRotated(-vec.y-center_pos[index2].y,center_pos[index2], ofVec3f(1,0,1));
                    
                    
                }
            }
            
        }
    }
    
}
void testApp::threadedFunction(){
    if(isThreadRunning())
    {
        //        ofSleepMillis(1000.0f/60.0f);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    light.enable();
    
    // begin scene to post process
    post.begin(cam);
    
    ofPushMatrix();

    ofScale(1, -1, 1);
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
    // end scene and draw
    post.end();
    
    // set gl state back to original
    glPopAttrib();
//    image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
//    if(saver.isRecording()) saver.writeRGB(image.getPixels());
    server.publishScreen();
    if(ofGetLogLevel()==OF_LOG_VERBOSE)
    {
        for (int j=0; j<GRID_HEIGHT; j++) {
            for (int i=0; i<GRID_WIDTH; i++) {
                
                int index2 = (j*GRID_WIDTH+i);
                ofCircle(center_pos[index2],10);
            }
        }
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 20,20);
    }

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'd')
    {
        if (ofGetLogLevel()!=OF_LOG_VERBOSE) {
            ofSetLogLevel(OF_LOG_VERBOSE);
        }
        else {
            ofSetLogLevel(OF_LOG_NOTICE);
        }
    }
    unsigned idx = key - '0';
    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());
    //    for (int j=0; j<GRID_HEIGHT; j++) {
    //        for (int i=0; i<GRID_WIDTH; i++) {
    //
    //            int index2 = (j*GRID_WIDTH+i);
    //            bStart[index2] = !bStart[index2];
    //        }
    //    }
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