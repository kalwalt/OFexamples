#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxDelaunay.h"
#include <vector>


class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        	ofEasyCam  easyCam;
        	ofxKinect  kinect;
        	float scaledVol;
        	ofxDelaunay delaunay;
private:
    		ofMesh mesh;		// ofVboMesh
    		vector < vector<ofVec3f> > points;
    		vector < vector<ofColor> > colors;
    		vector < vector<int> > indexs;


};
