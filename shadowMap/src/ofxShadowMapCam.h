#pragma once


#include "ofxShadowCamera.h"


class ofxShadowMapCam : public ofxShadowCamera {
private:
   bool mouseHasMoved, mouseClicked;
   float dmouseX, dmouseY, pmouseX, pmouseY;
   int mouseButton;
public:
   float zoomSpeed, orbitSpeed, panSpeed;

   ofxShadowMapCam() {
      zoomSpeed = 1;
      orbitSpeed = .2;
      panSpeed = 1;
      ofAddListener(ofEvents.mousePressed, this, &ofxShadowMapCam::mousePressed);
      ofAddListener(ofEvents.mouseReleased, this, &ofxShadowMapCam::mouseReleased);
      ofAddListener(ofEvents.mouseDragged, this, &ofxShadowMapCam::mouseDragged);
   }
   void setup(ofBaseApp* app, float dist) {
      position(ofGetWidth() / 2, ofGetHeight() / 2, dist);
      dmouseX = dmouseY = pmouseX = pmouseY = 0;
      mouseHasMoved = false;
      mouseClicked = false;
   }
	void setup(ofBaseApp* app, ofVec3f pos, ofVec3f eyepos) {
		position(pos);
		eye(eyepos);
		dmouseX = dmouseY = pmouseX = pmouseY = 0;
		mouseHasMoved = false;
		mouseClicked = false;
	}
   void draw() {
      ofVec3f relx = getDir().getCrossed(getUp()).normalize();
      ofVec3f rely = getDir().getCrossed(relx).normalize();
      if(mouseButton == 0) {
         orbitAround(
            getEye(),
            relx,
            orbitSpeed * dmouseY);
         orbitAround(
            getEye(),
            rely,
            orbitSpeed * -dmouseX);
      } else if(mouseButton == 2) {
         ofVec3f offset = getDir().normalize() * (zoomSpeed * dmouseY);
         eye(getEye() - offset);
         moveGlobal(offset);
      } else if(mouseButton == 3) {
         ofVec3f offset =
            (relx * dmouseX * panSpeed) +
            (rely * dmouseY * panSpeed);
         moveGlobal(offset);
      }
      dmouseX *= .9;
      dmouseY *= .9;
      place();
   }
   void mousePressed(ofMouseEventArgs& event) {
      if(mouseClicked == true)
         mouseButton = 3; // left + right
      else
         mouseButton = event.button;
      mouseClicked = true;
   }

   void mouseDragged(ofMouseEventArgs& event) {
      int x = event.x;
      int y = event.y;
      if(mouseHasMoved) {
         dmouseX = pmouseX - x;
         dmouseY = pmouseY - y;
      } else
         mouseHasMoved = true;
      pmouseX = x;
      pmouseY = y;
   }

   void mouseReleased(ofMouseEventArgs& event) {
      mouseHasMoved = false;
      mouseClicked = false;
   }
};
