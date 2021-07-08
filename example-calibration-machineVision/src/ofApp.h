#pragma once

#include "ofMain.h"

#include "aruco.h"
#include "ofxCv.h"

#include "calibrator.h"
#include "arucoGLUtils.h"

#include "ofxMachineVision.h"
#include "ofxSpinnaker.h"



class ofApp: public ofBaseApp
{
public:

    void setup();
	void update();
	void draw();
    void keyReleased(int key);

    // constants   
    const int videoDeviceId = 1;

    const string markerMap = "";
    const float markerSize = 0.038;

    const string calibrationFilePath = "calibration.yml";
    	
    // marker detection
    aruco::MarkerDetector detector;
    vector<aruco::Marker> markers;
    ofImage imgMarkersInfo;

    // grabber
    ofxMachineVision::SimpleGrabber<ofxMachineVision::Device::Spinnaker> grabber;
    cv::Size videoSize = cv::Size(640, 480);
    ofImage imgVideoUndistort;

    ofPixels pixGrabberScaled;

    // calibration
    aruco::Calibrator calibrator;
    aruco::CameraParameters camParams;

    bool bCapture = false;
    bool bSaveImages = false;

    void saveCalibration();

    // test calibration
    void drawCalibratedView();
    glm::mat4 projectionMatrix;
    bool bTestCalibration = false;
    bool bUndistort = false;
};
