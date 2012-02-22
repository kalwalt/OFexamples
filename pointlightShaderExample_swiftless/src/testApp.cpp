/* from swiftless tutorial http://www.swiftless.com/tutorials/glsl/5_lighting_perpixel.html#FS adapted to OpenFramework by kalwalt 21/02/2012
http://www.kalwaltart.it e-mail: info@kalwaltart.it
*/ 
#include "testApp.h"
#include <GL/glut.h>

void setupGlLight(){

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat lmKa[] = {1.0, 0.0, 0.2, 0.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat light_position[] = { 108, 20.0, 100.0, 0.0 }; //light directional if w=0 without shader works
    GLfloat light_position[] = { 220.0, 10.0, 0.0, 1.0 }; //light directional if w=0 without shader works

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

}

void setupGlMaterial(){

    GLfloat mat_ambient[] = {0.0f, 0.0f, 0.7f, 1.0f};
    GLfloat mat_diffuse[] = {0.1f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS, 40.0);

}

void testApp::setupOFlight(){

    light.enable();

    //uncomment this line and comment "light.setDirectionalLight();" if you want try pointlight
    //light.setPointLight();


    //we use a directional light...
    light.setDirectional();
    light.setPosition(10, 12,8);
    diffuseL.set(1.02f,1.1f,1.04f,1.0f);
    ambientL.set(0.2f,0.1f,0.0f,1.0f);
    specularL.set(.01f,.02f,.04f,.0f);

    light.setAmbientColor(diffuseL);
    light.setDiffuseColor(ambientL);
    light.setSpecularColor(specularL);


}

void testApp::setupOFmaterial(){

    diffuse.set(0.2f,0.5f,0.8f,1.0f);
    ambient.set(0.2f,0.20f,0.20f,1.0f);
    specular.set(0.0f,0.f,0.5f,1.0f);
    emissive.set(0.0f,0.0f,0.0f,1.0f);
    mat.setColors(diffuse,ambient,specular,emissive);
    mat.setShininess(20.1f);

}

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(34, 34, 34);

    //try another shader if you want...
    //shader.load("shaders/shader.vert", "shaders/shader.frag");
    shader.load("shaders/pointlight.vert", "shaders/pointlight.frag");

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);


    shader.begin();


    cam.begin();


    //use Opengl method for materials with Openframework function you can't use the shader...
    setupGlLight();
    setupGlMaterial();

    //setupOFlight();
    //setupOFmaterial();


    //ofTranslate(ofGetWidth()*0.5,ofGetHeight()*0.5,-10.0);


    //glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    ofScale(20,20,20);
    ofRotateX(180);
    glutSolidTeapot(10.0);

    shader.end();

    cam.end();
    //glutSwapBuffers();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

