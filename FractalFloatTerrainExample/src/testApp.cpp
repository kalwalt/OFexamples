#include "testApp.h"



void setupGlLight(){

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat lmKa[] = {0.8, 0.8, 0.8, 0.8 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { +100, 100, 100, 0.0 }; //light directional if w=0 without shader works
    //GLfloat light_position[] = { 220.0, 10.0, 0.0, 1.0 };

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
    glMaterialf(GL_FRONT,GL_SHININESS, 60.0);

}

void testApp::setupOFmaterial(){

    diffuse.set(0.4f,0.4f,0.4f,1.0f);
	ambient.set(0.4f,0.4f,0.4f,1.0f);
	specular.set(0.01f,0.2f,0.2f,1.0f);
	emissive.set(0.0f,0.0f,0.0f,1.0f);
    mat.setColors(diffuse,ambient,specular,emissive);
    mat.setShininess(120.1f);

}
void testApp::setupOFlight(){

    light.enable();
    light.draw();
    //light.setDirectional();
    light.setPointLight();

    light.setPosition(+100, 100, 100);
    light.setAttenuation(0.2,0.3,0.4);

    diffuseL.set(1.0f,1.0f,1.0f,1.0f);
	ambientL.set(0.0f,0.0f,0.0f,1.0f);
	specularL.set(1.0f,1.0f,1.0f,1.0f);

    light.setAmbientColor(diffuseL);
	light.setDiffuseColor(ambientL);
	light.setSpecularColor(specularL);

}

void testApp::add_Face(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {


    int rows = img.getWidth()*3;
	int cols = img.getHeight()*3;
    float spaceX = 8;
	float spaceY = 8;

    int sizeW = spaceX * rows;
    int sizeH = spaceY * cols;
    int width = img.getWidth();
	int height = img.getHeight();

	ofVec3f normal = ((b - a).cross(c - a)).normalize();

	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addTexCoord(ofVec2f(a.x/width,a.y/height));


	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addTexCoord(ofVec2f(b.x/width,b.y/height));

	mesh.addNormal(normal);
	mesh.addVertex(c);
	mesh.addTexCoord(ofVec2f(c.x/width,c.y/height));

}

//--------------------------------------------------------------
void testApp::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	add_Face(mesh, a, b, c);
	add_Face(mesh, a, c, d);
}

//--------------------------------------------------------------
ofVec3f getVertexFromImg(ofFloatImage& img, int x, int y) {
	float scaleFactor = 100;
	return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
}

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofDisableArbTex();
    ofEnableNormalizedTexCoords();


    DIFmap.loadImage("Floaterrain_TX.jpg");
    GLSmap.loadImage("Floaterrain_SpecLM.png");
    NRMmap.loadImage("Floaterrain_TN.png");

    img.loadImage("Float_Terrain.exr");
    //img.loadImage("mountains.raw");
    //bumpshader.load("bumpmap");
    bumpshader.load("NormalMap");
    //bumpshader.load("DiffBumpSpec");

     ofEnableLighting();
    light.enable();


	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 1;
	int width = img.getWidth();
	int height = img.getHeight();
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			ofVec3f nw = getVertexFromImg(img, x, y);
			ofVec3f ne = getVertexFromImg(img, x + skip, y);
			ofVec3f sw = getVertexFromImg(img, x, y + skip);
			ofVec3f se = getVertexFromImg(img, x + skip, y + skip);


			addFace(mesh, nw, ne, se, sw);



		}
	}


    setupGlLight();
    //setupOFlight();
    setupGlMaterial();
    //setupOFmaterial();
}

//--------------------------------------------------------------
void testApp::update(){



}

//--------------------------------------------------------------
void testApp::draw(){

    ofPushStyle();

        ofSetColor(255,0,0);
        ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);
        ofDrawBitmapString("num. vertices: "+ofToString(mesh.getNumVertices()),10,20);

    ofPopStyle();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

    glShadeModel(GL_SMOOTH);
    ofEnableLighting();

	easyCam.begin();
		ofScale(1, -1, 1);
		ofRotateX(60);
		ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2, 0);

        mat.begin();
		bumpshader.begin();

        bumpshader.setUniformTexture("colorMap",DIFmap,DIFmap.getTextureReference().getTextureData().textureID);
        bumpshader.setUniformTexture("glossMap",GLSmap,GLSmap.getTextureReference().getTextureData().textureID);
        bumpshader.setUniformTexture("normalMap",NRMmap,NRMmap.getTextureReference().getTextureData().textureID);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		mesh.draw();
		//mesh.drawWireframe();
    bumpshader.end();
    mat.end();
	easyCam.end();


   ofDisableLighting();
glDisable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
