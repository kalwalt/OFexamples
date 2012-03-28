/*
 *  ofxShadowMapUtil.cpp
 *  ofxShadowMapUtil
 *
 *  Created by Makira on 10/09/09.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxShadowMapUtil.h"


void ofxShadowMapUtil::setup(ofVec3f p_p_camera, ofVec3f p_l_camera, ofVec3f p_p_light, ofVec3f p_l_light,
							float p_render_width, float p_render_height,
							float p_shadow_map_coef, float p_blur_coef,
							bool p_bDebug) {

	bDebug = p_bDebug;

	render_width = p_render_width;
	render_height = p_render_height;
	shadow_map_coef = p_shadow_map_coef;
	blur_coef = p_blur_coef;

	p_camera = p_p_camera;
	l_camera = p_l_camera;
	p_light = p_p_light;
	l_light = p_l_light;

	// setup shader

	shadowShader.load("shaders/shadow");
	blurShader.load("shaders/Blur");
	storeDepthShader.load("shaders/StoreDepth");

	generateShadowFBO();

	// This is important, if not here, FBO's depthbuffer won't be populated.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);

	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void ofxShadowMapUtil::update(ofVec3f p_p_camera, ofVec3f p_l_camera, ofVec3f p_p_light, ofVec3f p_l_light) {

	p_camera = p_p_camera;
	l_camera = p_l_camera;
//	p_light = p_p_light;
//	l_light = p_l_light;

	//p_light[0] = 35.0 * cos(glutGet(GLUT_ELAPSED_TIME)/2000.0);
	//p_light[2] = 35.0 * sin(glutGet(GLUT_ELAPSED_TIME)/2000.0);

	p_light[0] = 35.0 * cos(ofGetElapsedTimef()/20.0);
	p_light[2] = 35.0 * sin(ofGetElapsedTimef()/20.0);

}


void ofxShadowMapUtil::beginLightPOVRendering() {

	//First step: Render from the light POV to a FBO, store depth and square depth in a 32F frameBuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen

	//Using the custom shader to do so
	//glUseProgramObjectARB(storeDepthShader.shader);
	//glUseProgramObjectARB(storeDepthShader.getShader(1));
	storeDepthShader.begin();

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0, render_width * shadow_map_coef, render_height * shadow_map_coef);

	// Clear previous frame values
	glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

	setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	//glCullFace(GL_FRONT);

}

void ofxShadowMapUtil::endLightPOVRendering() {

	glGenerateMipmapEXT(GL_TEXTURE_2D);
	//Save modelview/projection matrice into texture7, also add a biais
	setTextureMatrix();

	blurShadowMap();

}

void ofxShadowMapUtil::beginCameraPOVRendering() {

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);

	glViewport(0,0, render_width , render_height);

	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgramObjectARB(shadowShader.shader);
	shadowShader.begin();
	glUniform1iARB(glGetUniformLocationARB(shadowShader.getShader(GL_FRAGMENT_SHADER),"ShadowMap"), 7);
	//glUniform1fARB(glGetUniformLocationARB(shadowShader.shader,"xPixelOffset"), 1.0/ (render_width * shadow_map_coef));
	//glUniform1fARB(glGetUniformLocationARB(shadowShader.shader,"yPixelOffset"), 1.0/ (render_height * shadow_map_coef));

	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,colorTextureId);


	setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2]);

	float pl[3] = {p_light.x, p_light.y, p_light.z};
	glLightfv(GL_LIGHT0, GL_POSITION, pl);

	glCullFace(GL_BACK);

}


void ofxShadowMapUtil::endCameraPOVRendering() {

	if (bDebug) debugDraw();
	//glutSwapBuffers();

}


void ofxShadowMapUtil::generateShadowFBO() {

	int shadowMapWidth = render_width * shadow_map_coef;
	int shadowMapHeight = render_height * shadow_map_coef;

	//GLfloat borderColor[4] = {0,0,0,0};

	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&colorTextureId);
	glBindTexture(GL_TEXTURE_2D, colorTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);


	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F_ARB, shadowMapWidth, shadowMapHeight, 0, GL_RGB, GL_FLOAT, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);



	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT ,GL_TEXTURE_2D, depthTextureId, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, colorTextureId, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed for shadowmap FBO, CANNOT use FBO¥n");

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);



	// Creating the blur FBO
	glGenFramebuffersEXT(1, &blurFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, blurFboId);

	glGenTextures(1,&blurFboIdColorTextureId);
	glBindTexture(GL_TEXTURE_2D, blurFboIdColorTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F_ARB, shadowMapWidth * blur_coef, shadowMapHeight * blur_coef, 0, GL_RGB, GL_FLOAT, 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, blurFboIdColorTextureId, 0);
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed for blur FBO, CANNOT use FBO¥n");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void ofxShadowMapUtil::blurShadowMap() {

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	// Bluring the shadow map  horinzontaly
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,blurFboId);
	glViewport(0,0, render_width * shadow_map_coef * blur_coef, render_height * shadow_map_coef * blur_coef);
	//glUseProgramObjectARB(blurShader.shader);
	blurShader.begin();
	glUniform2fARB(glGetUniformLocationARB(blurShader.getShader(GL_FRAGMENT_SHADER),"ScaleU"),1.0/ (render_width * shadow_map_coef * blur_coef),0.0);		// Bluring horinzontaly
	glUniform1iARB(glGetUniformLocationARB(blurShader.getShader(GL_FRAGMENT_SHADER),"textureSource"),0);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,colorTextureId);

	//Preparing to draw quad
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-render_width/2, render_width/2, -render_height/2, render_height/2, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Drawing quad
	glTranslated(0,0,-5);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3f(-render_width/2,-render_height/2,0);
	glTexCoord2d(1,0);glVertex3f(render_width/2,-render_height/2,0);
	glTexCoord2d(1,1);glVertex3f(render_width/2,render_height/2,0);
	glTexCoord2d(0,1);glVertex3f(-render_width/2,render_height/2,0);
	glEnd();
	//glGenerateMipmapEXT(GL_TEXTURE_2D);


	// Bluring vertically
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);
	glViewport(0,0,render_width * shadow_map_coef ,render_height* shadow_map_coef);

	glUniform2fARB(glGetUniformLocationARB(blurShader.getShader(GL_FRAGMENT_SHADER),"ScaleU"),0.0, 1.0/ (render_height * shadow_map_coef));
	glBindTexture(GL_TEXTURE_2D,blurFboIdColorTextureId);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3f(-render_width/2,-render_height/2,0);
	glTexCoord2d(1,0);glVertex3f(render_width/2,-render_height/2,0);
	glTexCoord2d(1,1);glVertex3f(render_width/2,render_height/2,0);
	glTexCoord2d(0,1);glVertex3f(-render_width/2,render_height/2,0);
	glEnd();

}

void ofxShadowMapUtil::startTranslate(float x,float y,float z) {

	glPushMatrix();
	glTranslatef(x,y,z);

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(x,y,z);
}

void ofxShadowMapUtil::endTranslate() {

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void ofxShadowMapUtil::setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, render_width/render_height, 10, 120);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);


}

void ofxShadowMapUtil::setTextureMatrix() {

	static double modelView[16];
	static double projection[16];

	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5
	// y = y* 0.5 + 0.5
	// z = z* 0.5 + 0.5
	// Moving from unit cube [-1,1] to [0,1]
	const GLdouble bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);


	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixd(bias);

	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);

}


void ofxShadowMapUtil::debugDraw() {
	glUseProgramObjectARB(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-render_width/2,render_width/2,-render_height/2,render_height/2,1,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1,1,1,1);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,blurFboIdColorTextureId);
	glEnable(GL_TEXTURE_2D);
	glTranslated(0,0,-1);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3f(0,0,0);
	glTexCoord2d(1,0);glVertex3f(render_width/2,0,0);
	glTexCoord2d(1,1);glVertex3f(render_width/2,render_height/2,0);
	glTexCoord2d(0,1);glVertex3f(0,render_height/2,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
