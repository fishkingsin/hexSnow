#pragma once

#include "ofMain.h"
#define GRID_WIDTH  16
#define GRID_HEIGHT 16
#define LENGTH		6
#include "ofxPostProcessing.h"
#define RANGE 300
#include "ofxQTKitVideoSaver.h"
#include "ofxSyphonServer.h"
class testApp : public ofBaseApp , public ofThread{

	public:
    void exit();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void threadedFunction();
    ofVbo vbo;
	ofVec3f pos[GRID_WIDTH*GRID_HEIGHT*LENGTH];
    ofVec3f loc[GRID_WIDTH*GRID_HEIGHT*LENGTH];

	ofVec2f tex_coord[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	ofFloatColor color[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	ofVec3f center;
	int total;
	int space,space2;
    int SIZE;
    bool bStart[GRID_WIDTH*GRID_HEIGHT];
    ofVec3f center_pos[GRID_WIDTH*GRID_HEIGHT];
    ofVec3f acc[GRID_WIDTH*GRID_HEIGHT];
    int count;
    float timeCount;
    
    ofLight light;
    
    // scene stuff
    ofxPostProcessing post;
        ofEasyCam cam;
//    ofxQTKitVideoSaver	 saver;
//    ofImage image;
    ofxSyphonServer server;
};
