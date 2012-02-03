//made by kalwaltart 01/02/2012 

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


	ofSetLogLevel(OF_LOG_VERBOSE);

	//we set here our Fbo

    ofFbo::Settings set;

    //REMEMBER !!! : texture width and height MUST be in power of 2, id est 2,4,8,16,32,64,128 and so on, otherwise your output will be black and weird in some parts...!

   set.width = 256;
   set.height = 256;
   //set.internalformat = GL_RGBA8;
   set.internalformat = GL_RGBA;
   set.maxFilter = GL_LINEAR;
   set.minFilter = GL_LINEAR;
   //set.minFilter = GL_LINEAR_MIPMAP_LINEAR;
   set.numColorbuffers = 1;
   set.numSamples = 3;
   //set.textureTarget = GL_TEXTURE_RECTANGLE;
   set.textureTarget = GL_TEXTURE_2D;
   set.useDepth = true;
   set.useStencil = true;
   set.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
   set.wrapModeVertical = GL_CLAMP_TO_EDGE;

    //allocating the fbo

   fbo.allocate(set);

   //mipmap will be necessary but for another time...
   //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    //we load the shader and some material and light setings

    shader.load("multiLightShader.vert", "multiLightShader.frag");
    material.setShininess( 100 );
    light.enable();


}

//--------------------------------------------------------------
void testApp::update(){

    //we do something in our fbo

    fbo.begin();
        ofClear(255, 255, 255, 255);
        ofSetColor(255, 0, 0);
        ofCircle(mouseX, mouseY, 20);
    fbo.end();


}

//--------------------------------------------------------------
void testApp::draw(){

    glEnable(GL_DEPTH_TEST);


    ofEnableLighting();
	light.enable();
	light.setPosition(ofGetWidth()*.5, ofGetHeight()*.5, 300 );

    //inside the shader we make the magik stuff : Fbo  onto the ofBox!

    material.begin();
	shader.begin();
	shader.setUniformTexture("baseMap", fbo.getTextureReference(), fbo.getTextureReference().getTextureData().textureID);

        ofPushMatrix();

            ofTranslate(ofGetWindowWidth()*0.5,ofGetWindowHeight()*0.5,0);

            ofRotateX(ofGetElapsedTimef() * 30);
            ofRotateY(ofGetElapsedTimef() * 30);
            ofScale(20,20,20);


            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            ofBox(0,0,10);


        ofPopMatrix();

    shader.end();
    material.end();


	light.disable();
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);

    //we display the entire Fbo also onto the window, just for fun!

    ofSetColor(255);
    fbo.draw(0, 0);
}

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
