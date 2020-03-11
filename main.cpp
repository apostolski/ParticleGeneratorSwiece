#include "ofMain.h"
#include "ofApp.h"
#include "omp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);
	ofRunApp(new ofApp());
}
