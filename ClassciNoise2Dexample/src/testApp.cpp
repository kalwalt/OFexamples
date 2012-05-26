#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofBackground(127,210,10);

   	w=256;
    	h=256;


	heightshader.setupShaderFromFile(GL_VERTEX_SHADER,"noisebench.vert");
   
	heightshader.setupShaderFromFile(GL_FRAGMENT_SHADER,"classicnoise2D.frag");
	heightshader.linkProgram();

	




}

//--------------------------------------------------------------
void testApp::update(){
  heightshader.begin();

  
  heightshader.setUniform1f("time", time);

 heightshader.end();
 time =ofGetElapsedTimef()*0.5;

}

//--------------------------------------------------------------
void testApp::draw(){

glDisable(GL_DEPTH_TEST);
 glClearColor(0.0, 0.0, 0.0, 1.0);
   
    heightshader.begin();

int n =1;

  glColor3f(1.0f, 1.0f, 1.0f); // White base color
    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(-n, -n);
	glVertex2f(0, 0);

	glTexCoord2f(-n, n);
	glVertex2f(0, h);

	glTexCoord2f(n, -n);
	glVertex2f(w, 0);

	glTexCoord2f(n, n);
	glVertex2f(w, h);

    glEnd();


    heightshader.end();
   

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//------------- -------------------------------------------------
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
