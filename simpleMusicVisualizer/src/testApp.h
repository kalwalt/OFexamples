
#pragma once

/*
 This example demonstrates the emmiter which borns the particles.
 
 It's the example 02-ParticlesEmitter from the book
 "Mastering openFrameworks: Creative Coding Demystified",
 Chapter 3 - Building a Simple Particle System
 */

#include "ofMain.h"


//Particle class
class Particle {
public:
    Particle();                //Class constructor
    void setup();              //Start particle
    void setupImage();
    void update( float dt );   //Recalculate physics
    void draw();               //Draw particle
    void fftSet();
    void fftUpdate();
    
    ofPoint pos;               //Position
    ofPoint vel;               //Velocity
    float time;                //Time of living
    float lifeTime;            //Allowed lifetime
    bool live;                 //Is particle live
    
    ofImage glow,glow2;
    
    int     nBandsToGet;
    float * fftSmoothed;
    int     fftchosen;
    
};

//Control parameters class
class Params {
public:
    void setup();
    ofPoint eCenter;    //Emitter center
    float eRad;         //Emitter radius
    float velRad;       //Initial velocity limit
    float velRad2;
    float lifeTime;     //Lifetime in seconds
    
    float rotate;   //Direction rotation speed in angles per second
};

extern Params param; //Declaration of a global variable


//openFrameworks' class
class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    vector<Particle> p;	  //Particles
    
    float history;        //Control parameter for trails
    float time0;          //Time value for computing dt
    
    float bornRate;       //Particles born rate per second
    float bornCount;      //Integrated number of particles to born
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofSoundPlayer	song1;
    ofImage         glow3,glow4,glow5;
    ofColor         color1,color2;
    
    int             nBandsToGet2;
    float         * fftSmoothed2;
    int             fftchosen2;
    
    
    
    
};
