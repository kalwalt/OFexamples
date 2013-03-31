#include "testApp.h"

#define LINE_WEIGHT 1.2

//--------------------------------------------------------------
void testApp::setup(){

    kinect.init();
    kinect.open();

    kinect.setDepthClipping(0,500);

    kinect.setRegistration(true);

    // Kinect 初期の首角度
    kinect.setCameraTiltAngle(12);
    scaledVol =.05;
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
    	//clear
	mesh.clear();
	points.clear();
	colors.clear();
	indexs.clear();

	int step = int(5 + int(scaledVol*15));
	int total = 0;
	for (int j = 0; j < kinect.height; j+=step){

    		vector <ofVec3f>temppoints;
    		vector <ofColor>tempcolors;
    		points.push_back(temppoints);
    		colors.push_back(tempcolors);

    		for (int i = 0; i < kinect.width; i+=step){

        float distance = kinect.getDistanceAt(i, j);

        if(distance>50 && distance<1000){

            ofVec3f tempPoint;
            ofColor tempColor;

            tempPoint = ofVec3f(i, j, distance*-2.0 );
            tempColor = ofColor(kinect.getColorAt(i,j));

            points[j/step].push_back(tempPoint);
            colors[j/step].push_back(tempColor);

            total++;

            }else{

            ofVec3f tempPoint2;
            ofColor tempColor2;
            tempPoint2 = ofVec3f(i,j,0);
            tempColor2 = ofColor(0);
            points[j/step].push_back(tempPoint2);
            colors[j/step].push_back(tempColor2);
        }
    }
}

	for(int i = 0; i < points.size();i++){


	delaunay.addPoint(points[i][0][0],points[i][0][0],points[i][0][0]);
	}
	delaunay.triangulate();

	int ind = 0;
	for (int m = 0; m < kinect.height; m+=step)
	{
    	vector <int>tempindexs;
    	indexs.push_back(tempindexs);

    	for(int n = 0; n < kinect.width; n+=step){

        if(points[m/step][n/step].z != 0){
//          cout << points[m][n] << endl;

            mesh.addColor(colors[m/step][n/step]);
            mesh.addVertex(points[m/step][n/step]);

            indexs[m/step].push_back(ind);

            ind++;
            }else{

            indexs[m/step].push_back(-1);
            }
    }
}



	int W = int(kinect.width/step);
	for(int b = 0; b < kinect.height-step; b+=step){
    		for(int a = 0; a < kinect.width-1; a+=step)
   		 {
        		if( (indexs[int(b/step)][int(a/step)]!=-1 && indexs[int(b/step)][int(a/step+1)]!=-1) && (indexs[int(b/step+1)][int(a/step+1)]!=-1 && indexs[int(b/step+1)][int(a/step)]!=-1) ){
            mesh.addTriangle(indexs[int(b/step)][int(a/step)],indexs[int(b/step)][int(a/step+1)],indexs[int(b/step+1)][int(a/step+1)]);
            mesh.addTriangle(indexs[int(b/step)][int(a/step)],indexs[int(b/step+1)][int(a/step+1)],indexs[int(b/step+1)][int(a/step)]);

        }

    }
}

}

//--------------------------------------------------------------
void testApp::draw(){

    //// draw ////

glEnable(GL_DEPTH_TEST);
//easyCam.begin();
//ofTranslate(ofGetWidth()*.5f,ofGetHeight()*.5f,10.0f);
ofScale(4,4,4);
//ofRotateY(180);
mesh.setMode(OF_PRIMITIVE_TRIANGLES);
glLineWidth(int(LINE_WEIGHT));
mesh.drawWireframe();
//easyCam.end();

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
