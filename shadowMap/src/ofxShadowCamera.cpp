#include "ofxShadowCamera.h"


ofxShadowCamera::ofxShadowCamera(){
	perspective();
	position();
	eye();
	up();
}

void ofxShadowCamera::position(float x, float y, float z){
	posCoord.x = x;
	posCoord.y = y;
	posCoord.z = z;
}
void ofxShadowCamera::position(ofVec3f _pos){
	posCoord = _pos;
}
void ofxShadowCamera::position(){
	posCoord.x = (float)w/2.0f;
	posCoord.y = (float)h/2.0f;
	float halfFov = PI * fieldOfView / 360.0f;
	float theTan = tanf(halfFov);
	posCoord.z = posCoord.y/theTan;
}

void ofxShadowCamera::lerpPosition(float _targetX, float _targetY, float _targetZ, float _step){
	posCoord.x += (_targetX - posCoord.x) * _step;
	posCoord.y += (_targetY - posCoord.y) * _step;
	posCoord.z += (_targetZ - posCoord.z) * _step;
}

void ofxShadowCamera::lerpPosition(ofVec3f target, float step){
	lerpPosition(target.x, target.y, target.z, step);
}

void ofxShadowCamera::eye(float x, float y, float z){
	eyeCoord.x = x;
	eyeCoord.y = y;
	eyeCoord.z = z;
}

void ofxShadowCamera::eye(ofVec3f _pos){
	eyeCoord = _pos;
}

void ofxShadowCamera::eye(){
	eyeCoord.x = (float)w/2.0f;
	eyeCoord.y = (float)h/2.0f;
	eyeCoord.z = 0;
}


void ofxShadowCamera::up(float _nx, float _ny, float _nz){
	upVec.x = _nx;
	upVec.y = _ny;
	upVec.z = _nz;
}

void ofxShadowCamera::up(ofVec3f _up){
	upVec = _up;
}


void ofxShadowCamera::up(){
	upVec.x = 0;
	upVec.y = 1;
	upVec.z = 0;
}

void ofxShadowCamera::perspective(float _fov, float _aspect, float _zNear, float _zFar){
	fieldOfView = _fov;
	aspectRatio = _aspect;
	if(_zNear==0) _zNear = 0.01;
	zNear = _zNear;
	zFar = _zFar;
}

void ofxShadowCamera::perspective(){
	fieldOfView = 65.0f;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	aspectRatio = (float)w/(float)h;
	zNear = 1.0f;
	zFar = 10000.0f;
}

void ofxShadowCamera::place(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posCoord[0], posCoord[1], posCoord[2], eyeCoord[0], eyeCoord[1], eyeCoord[2], upVec[0], upVec[1], upVec[2]);

	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
}


//Removes the camera, so it returns as if there was no camera
void ofxShadowCamera::remove(){
	ofSetupScreen();
}



void ofxShadowCamera::moveLocal(float _x, float _y, float _z){
	moveLocal(ofVec3f(_x, _y, _z));
}

void ofxShadowCamera::moveLocal(ofVec3f move){
	ofVec3f dir =  getDir().normalized();
	posCoord += dir.rescaled(move.z);
	eyeCoord += dir.rescaled(move.z);

	posCoord += upVec.rescaled(move.y);
	eyeCoord += upVec.rescaled(move.y);

	posCoord += dir.cross(upVec).rescaled(move.x);
	eyeCoord += dir.cross(upVec).rescaled(move.x);
}

void ofxShadowCamera::moveGlobal(float _x, float _y, float _z){
	posCoord.x += _x;
	posCoord.y += _y;
	posCoord.z += _z;
	eyeCoord.x += _x;
	eyeCoord.y += _y;
	eyeCoord.z += _z;
}
void ofxShadowCamera::moveGlobal(ofVec3f move){
	posCoord += move;
	eyeCoord += move;
}

void ofxShadowCamera::orbitAround(ofVec3f target, ofVec3f axis, float angle){
	ofVec3f r = posCoord-target;
	posCoord = target + r.rotated(angle, axis);
}

void ofxShadowCamera::rotate(ofVec3f axis, float angle){
	ofVec3f r = -posCoord+eyeCoord;
	eyeCoord = posCoord + r.rotated(angle, axis);
}

//
//Getters
//
ofVec3f ofxShadowCamera::getDir(){
	return eyeCoord-posCoord;
}

ofVec3f ofxShadowCamera::getPosition(){
	return posCoord;
}

ofVec3f ofxShadowCamera::getEye(){
	return eyeCoord;
}

ofVec3f ofxShadowCamera::getUp(){
	return upVec;
}


