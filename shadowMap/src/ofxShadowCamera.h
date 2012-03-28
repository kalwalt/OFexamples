#pragma once

#include "ofMain.h"
#include <GL/glut.h>

class ofxShadowCamera{
public:
	ofxShadowCamera();

	void position(float x, float y, float z);
	void position(ofVec3f _pos);
	void position(); //reset the position to initial values
	void lerpPosition(float _x, float _y, float _z, float _step); //step should be a value between 0 and 1
	void lerpPosition(ofVec3f _pos, float _step); //step should be a value between 0 and 1

	void eye(float _x, float _y, float _z);
	void eye(ofVec3f _eye);
	void eye(); //reset eye psition to the initial values
	void up(float _x, float _y, float _z);
	void up(ofVec3f _up);
	void up(); //reset up vector to initial values

	void perspective(float _fov, float _aspect, float _zNear, float _zFar);
	void perspective();//reset perspective to initial values

	void place(); //this must go in the draw function!!!!
	void remove(); //Removes the camera, so it returns as if there was no camera

	void moveLocal(float _x, float _y, float _z); //Moves the camera along it's own coordinatesystem
	void moveLocal(ofVec3f move);
	void moveGlobal(float _x, float _y, float _z); //Moves the camera along the global coordinatesystem
	void moveGlobal(ofVec3f move);

	void orbitAround(ofVec3f target, ofVec3f axis, float value);
	void rotate(ofVec3f axis, float value);

	ofVec3f getDir();
	ofVec3f getPosition();
	ofVec3f getEye();
	ofVec3f getUp();

private:
	ofVec3f posCoord;
	ofVec3f eyeCoord;
	ofVec3f upVec;

	//relative to defining the persperctive:
	float	fieldOfView;
	int	w;
	int	h;
	float	aspectRatio;
	float zNear, zFar;
};



