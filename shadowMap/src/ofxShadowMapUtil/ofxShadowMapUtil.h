#pragma once

/*
 *  ofxShadowMapUtil.h
 *  ofxShadowMapUtil
 *
 *  Created by Makira on 10/09/09.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
//#include "ofxVectorMath.h"
//#include "ofxShader.h"


class ofxShadowMapUtil {

public:

	void setup(ofVec3f p_p_camera = ofVec3f(35, 25, 5),
			   ofVec3f p_l_camera = ofVec3f(0, 0, -10),
			   ofVec3f p_p_light = ofVec3f(3, 18, 0),
			   ofVec3f p_l_light = ofVec3f(0, 0, -5),
			   float p_render_width = ofGetWidth(), float p_render_height = ofGetHeight(),
			   float p_shadow_map_coef = 0.5, float p_blur_coef = 0.25,
			   bool p_bDebug = false);
	void update(ofVec3f p_p_camera = ofVec3f(35, 25, 5),
				ofVec3f p_l_camera = ofVec3f(0, 0, -10),
				ofVec3f p_p_light = ofVec3f(3, 18, 0),
				ofVec3f p_l_light = ofVec3f(0, 0, -5));
	void beginLightPOVRendering();
	void endLightPOVRendering();
	void beginCameraPOVRendering();
	void endCameraPOVRendering();
	void startTranslate(float x,float y,float z);
	void endTranslate();

	inline void toggleDebug(bool p_bDebug) { bDebug = p_bDebug; }

protected:

	void setupMatrices(float position_x, float position_y, float position_z, float lookAt_x, float lookAt_y, float lookAt_z);
	void setTextureMatrix();
	void generateShadowFBO();
	void blurShadowMap();

	float		render_width;
	float		render_height;
	float		shadow_map_coef;
	float		blur_coef;
	float		light_mvnt;

	ofShader	shadowShader;
	ofShader	blurShader;
	ofShader 	storeDepthShader;

	GLuint		fboId;
	GLuint		blurFboId;
	GLuint		blurFboIdColorTextureId;
	GLuint		depthTextureId;
	GLuint		colorTextureId;

	ofVec3f	p_camera;
	ofVec3f	l_camera;	// camera lookat direction
	ofVec3f	p_light;
	ofVec3f	l_light;	// light lookat direction

private:

	void debugDraw();
	bool bDebug;

};
