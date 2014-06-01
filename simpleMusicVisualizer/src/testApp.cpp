


// Copyright (c) 2013 Manish Juneja
// License: The MIT License (MIT)
// MIT License page: http://opensource.org/licenses/MIT

#include "testApp.h"


//----------------------  Params -------------------------------
Params param;        //Definition of global variable
Particle newP;

void Params::setup() {
    eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 );
    eRad = 1;
    velRad = 400;
    velRad2 = 400;
    lifeTime = 1.0;
    
    rotate = 0;
}

//----------------------  Particle  ----------------------------
Particle::Particle() {
    live = false;
}

//--------------------------------------------------------------
ofPoint randomPointInCircle(){
    ofPoint pnt;
    float rad = 400;
    float angle = ofRandom( 0, M_TWO_PI );
    pnt.x = cos( angle ) * rad;
    pnt.y = sin( angle ) * rad;
    
    return pnt;
}

//--------------------------------------------------------------
void Particle::setup() {
    pos = param.eCenter;
    vel =  randomPointInCircle();
    time = 0;
    lifeTime = param.lifeTime;
    live = true;
    
    
}

//--------------------------------------------------------------
void Particle::update( float dt ){
    if ( live ) {
        //Update pos
        pos += vel * dt;    //Euler method
        //Update time and check if particle should die
        time += dt;
        if ( time >= lifeTime ) {
            live = false;   //Particle is now considered as died
        }
        
    }
    
}

void Particle::fftUpdate(){
    //---for---fft-----------------------
    float * val = ofSoundGetSpectrum(nBandsToGet);	 // request the number of  values(255) for fft
    for (int i = 0;i < nBandsToGet; i++){
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.4f;
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
        
    }
    fftchosen = (fftSmoothed[3] * 200);
    
}

//--------------------------------------------------------------
void Particle::draw(){
    if ( live ) {
        
        
        
        unsigned char * pix = glow.getPixels();
        float numPix = glow.width * glow.height * 4;
        
        float x =ofMap(fftchosen,5,250,100,250);
        
        for(int i = 0; i <numPix;  i+=4){
            if(pix[i] != 0)    pix[i] = x;
            if(pix[i+1] != 0)   pix[i+1] = x;
            if(pix[i+2] != 0)  pix[i+2] = x;
        }
        glow.update();
        
        float size = ofMap(fftchosen,5,250,20,90);
        
        glow.resize(size,size);
        glow.draw(pos,size,size);
        glow.update();
        
    }
    
    
    
}


//----------------------  testApp  -----------------------------
void Particle::setupImage(){
    
    glow.loadImage("circle_gradient2.png");
    glow.resize(20,20);
    
    glow2.loadImage("circle_gradient2.png");
    glow2.resize(300,300);
    glow2.setAnchorPoint(glow2.getWidth()/2,glow2.getHeight()/2);
}

void Particle::fftSet(){
    //fft
    fftSmoothed = new float[800];
    for (int i = 0; i < 800; i++){
        fftSmoothed[i] = 0;
    }
    
    nBandsToGet = 255;
}

void testApp::setup(){
    ofSetFrameRate( 120 );	//Set screen frame rate
    ofBackground( 0,0,0);
    color1.set(0, 0, 0);
    color2.set(0, 10, 60);
    
    
    newP.setupImage();
    newP.fftSet();
    
    glow3.loadImage("circle_gradient2.png");
    glow3.resize(200,200);
    glow3.setAnchorPoint(glow3.getWidth()/2,glow3.getHeight()/2);
    
    glow4.loadImage("little_glow.png");
    glow4.resize(700,700);
    glow4.setAnchorPoint(glow4.getWidth()/2,glow4.getHeight()/2);
    
    
    
    // for centre particle-------
    fftSmoothed2 = new float[800];
    for (int i = 0; i < 800; i++){
        fftSmoothed2[i] = 0;
    }
    
    nBandsToGet2 = 255;
    
    //Allocate drawing buffer
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    //loading sound
    //   song1.loadSound("jdee_beat.mp3");
    song1.loadSound("Tribute (Original Mix).mp3");
    
    
    //Set up parameters
    param.setup();	 //Global parameters
    history = 1;
    bornRate = 2;
    
    bornCount = 0;
    time0 = ofGetElapsedTimef();
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    newP.fftUpdate();
    
    //---for---fft-----------------------
    float * val2 = ofSoundGetSpectrum(nBandsToGet2);	 // request the number of  values(255) for fft
    for (int i = 0;i < nBandsToGet2; i++){
        // let the smoothed calue sink to zero:
        fftSmoothed2[i] *= 0.4f;
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed2[i] < val2[i]) fftSmoothed2[i] = val2[i];
        
    }
    fftchosen2 = (fftSmoothed2[3] * 200);
    
    
    //Compute dt
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;
    
    //Delete inactive particles
    int i=0;
    while (i < p.size()) {
        if ( !p[i].live ) {
            p.erase( p.begin() + i );
        }
        else {
            i++;
        }
    }
    
    //Born new particles
    bornCount += dt * bornRate;      //Update bornCount value
    if ( bornCount >= 1 ) {          //It's time to born particle(s)
        int bornN = int( bornCount );//How many born
        bornCount -= bornN;          //Correct bornCount value
        for (int i=0; i<bornN; i++) {
            
            newP.setup();            //Start a new particle
            p.push_back( newP );     //Add this particle to array
        }
    }
    
    //Update the particles
    for (int i=0; i<p.size(); i++) {
        p[i].update( dt );
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofEnableAlphaBlending();
    
    for (int i=0; i<p.size(); i++) {
        p[i].draw();
    }
    
    glow5 = glow3;
    
    
    unsigned char * pix5 = glow5.getPixels();
    float numpix5 = glow5.width * glow5.height * 4;
    
    float x =ofMap(fftchosen2,5,250,5,250);
    
    for(int i = 0; i <numpix5;  i+=4){
        if(pix5[i] != 0)    if(pix5[i]<x) pix5[i] = x;
        if(pix5[i+1] != 0)  if(pix5[i]<x) pix5[i+1] = x;
        if(pix5[i+2] != 0)  if(pix5[i]<x) pix5[i+2] = x;
    }
    glow5.update();
    
    float size = ofMap(fftchosen2,5,250,200,400);
    glow5.setAnchorPoint(size/2,size/2);
    glow5.resize(size,size);
    
    glow5.draw(ofGetWidth()/2,ofGetHeight()/2);
    
    glow4.draw(ofGetWidth()/2,ofGetHeight()/2);
    
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 40,50);
    ofDrawBitmapString(ofToString(bornRate) + "particles", 40,60);
    ofDrawBitmapString( "press a to play music", 40,70);
    ofDrawBitmapString( "press b to stop music", 40,80);
    ofDrawBitmapString( "press w to increase particles", 40,90);
    ofDrawBitmapString( "press s to decrease particles", 40,100);
    
    
    
    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'a' || key =='A'){
        song1.play();
        bornRate =22;
    }
    if(key == 'b' || key =='B'){
        song1.stop();
        bornRate =1;
    }
    
    if(key == 's' || key == 'S'){
        
        bornRate   -= 1;
        
    }
    if(key == 'w' || key == 'W') {
        
        bornRate +=1;
    }
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
