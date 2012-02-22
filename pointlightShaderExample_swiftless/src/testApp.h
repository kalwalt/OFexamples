#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void setupOFlight();
		void setupOFmaterial();

		ofShader shader;

		ofLight light;
		ofMaterial mat;

		ofFloatColor diffuse;
		ofFloatColor ambient;
		ofFloatColor specular;
		ofFloatColor emissive;

        	ofFloatColor diffuseL;
		ofFloatColor ambientL;
		ofFloatColor specularL;
		ofFloatColor emissiveL;

		ofEasyCam cam;

};

