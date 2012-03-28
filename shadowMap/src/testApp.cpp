#include "testApp.h"
#include <GL/glut.h>


//--------------------------------------------------------------
void testApp::setup(){

	// dont know why i need this line...
	// but w/o this line, i cannot read shader files(.frag, .vert)
	//ofSetDataPathRoot("/home/walter/openframeworks007/apps/examples/shadowmap/bin/data");

	su.setup();


	camera.setup(this, ofVec3f(35, 25, 5), ofVec3f(10, 0, -20));

}


//--------------------------------------------------------------
void testApp::update(){

	camera.draw();

	su.update(camera.getPosition(), camera.getEye());

}

//--------------------------------------------------------------
void testApp::draw(){


	// for light
	su.beginLightPOVRendering();
	drawObjects();
	su.endLightPOVRendering();

	// for cam
	su.beginCameraPOVRendering();
	drawObjects();
	su.endCameraPOVRendering();


	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::drawObjects() {

	// Ground
	//glColor4f(0.3f,0.3f,0.3f,1);
	ofSetColor(255, 255, 255);
	glBegin(GL_QUADS);
	glVertex3f(-45,2,-45);
	glVertex3f(-45,2, 55);
	glVertex3f( 55,2, 55);
	glVertex3f( 55,2,-45);
	glEnd();

	//glColor4f(0.9f,0.9f,0.9f,1);
	ofSetColor(224, 255, 255);

	// Instead of calling glTranslatef(or ofTranslate), we need a custom function that also maintain the light matrix
	su.startTranslate(0,8,-16);
	glutSolidCube(4);
	su.endTranslate();

	su.startTranslate(-8,4,-16);
	glutSolidCube(4);
	su.endTranslate();

	su.startTranslate(8,4,-16);
	glutSolidCube(4);
	ofCircle(0, 0, 5);
	su.endTranslate();

	su.startTranslate(0,8,-5);
	//glutSolidCube(4);
	glutSolidSphere(4,40,40);
	su.endTranslate();

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if (key = ' ') {
		su.toggleDebug(true);
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

	if (key = ' ') {
		su.toggleDebug(false);
	}

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
