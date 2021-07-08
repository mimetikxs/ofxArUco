#include "ofApp.h"

using namespace glm;
using namespace ofxCv;


void ofApp::setup()
{
    // init detector
    //detector.loadParamsFromFile(ofToDataPath("arucoConfig.yml"));
    detector.setDictionary("ARUCO_MIP_36h12");
    detector.setDetectionMode(aruco::DM_NORMAL);

    // init video grabber
    auto deviceDescriptions = grabber.getDevice()->listDevices();
    for (auto deviceDescription : deviceDescriptions)
        cout << deviceDescription.manufacturer << ", " << deviceDescription.model << endl;

    grabber.open();
    grabber.startCapture();

    // get the size of the video
    auto specs = grabber.getDeviceSpecification();
    videoSize.width = specs.getCaptureWidth();
    videoSize.height = specs.getCaptureHeight();

    // init calibrator
    calibrator.setParams(videoSize, markerSize, markerMap);
    // testing..
    camParams.readFromXMLFile(ofToDataPath(calibrationFilePath));

    // undistorted
    imgVideoUndistort.allocate(videoSize.width, videoSize.height, OF_IMAGE_GRAYSCALE);

    // debug info
    imgMarkersInfo.allocate(videoSize.width, videoSize.height, OF_IMAGE_COLOR);

    // oF app
    ofSetVerticalSync(true);
    //ofSetFrameRate(30);
    ofSetWindowShape(videoSize.width, videoSize.height);
}


void ofApp::update()
{
    grabber.update();

    if (grabber.isFrameNew())
    {
        // detect markers
        markers = detector.detect(toCv(grabber.getPixels()), camParams, markerSize, false);

        if (bTestCalibration) 
            return;

        // print info
        {
            // if we want info image to be mono like grabber's pixels
            //imageMarkersInfo.setFromPixels(grabber.getPixels());

            // if we want info image to be color
            auto channelPixel = grabber.getPixels().begin();
            for (auto p : imgMarkersInfo.getPixels().getPixelsIter()) {
                p[0] = *channelPixel;
                p[1] = *channelPixel;
                p[2] = *channelPixel++;
            }

            for (size_t i = 0; i < markers.size(); i++)
            {
                //ofLog() << markers[i];
                markers[i].draw(toCv(imgMarkersInfo), cv::Scalar(255, 10, 10), 2, false);
            }
            cv::putText(toCv(imgMarkersInfo), calibrator.getInfo(), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5f, cv::Scalar(125, 255, 255), 1);

            imgMarkersInfo.update();
        }

        // capture
        if (bCapture)
        {
            bCapture = false;

            calibrator.addView(markers);
            if (bSaveImages)
            {
                string number = std::to_string(calibrator.getNumberOfViews() - 1);
                while (number.size() != 5) number = "0" + number;
                imgMarkersInfo.save("calib-" + number + ".png");
            }
        }
    }
}


void ofApp::draw()
{    
    if (bTestCalibration)
        drawCalibratedView();
    else
        imgMarkersInfo.draw(0, 0);

   ofDrawBitmapStringHighlight("app fps: " + ofToString(ofGetFrameRate(), 0), 20, ofGetHeight() - 60);
   ofDrawBitmapStringHighlight("cam fps: " + ofToString(grabber.getFps(), 0), 20, ofGetHeight() - 40);
}


void ofApp::saveCalibration()
{
    if (calibrator.getCalibrationResults(camParams))
    {
        camParams.saveToFile(ofToDataPath(calibrationFilePath));
        cout << "results saved to " << calibrationFilePath << endl;
    }
    else cerr << "Could not obtain calibration" << endl;
    cout << "Final error=" << calibrator.getReprjError() << endl;
}


void ofApp::drawCalibratedView()
{
    if (bUndistort)
    {
        cv::undistort(toCv(grabber), toCv(imgVideoUndistort), camParams.CameraMatrix, camParams.Distorsion);
        imgVideoUndistort.update();
        imgVideoUndistort.draw(0, 0);   
    }
    else
    {
        grabber.draw(0, 0);
    }

    ofPushView();
    {
        ofViewport(0, 0, grabber.getWidth(), grabber.getHeight());

        ofSetMatrixMode(OF_MATRIX_PROJECTION);
        ofLoadMatrix(projectionMatrix);

        ofSetMatrixMode(OF_MATRIX_MODELVIEW);

        for (int i = 0; i < markers.size(); i++)
        {
            ofPushMatrix();
            ofLoadMatrix(ofxAruco::getModelViewMatrix(markers[i]));
            {
                ofDrawAxis(markerSize);

                ofTranslate(0, 0, markerSize * 0.5);

                ofPushStyle();
                ofSetColor(255, 100, 20, 50);
                ofDrawBox(markerSize);
                ofNoFill();
                ofSetColor(255, 100, 20);
                ofDrawBox(markerSize);
                ofPopStyle();
            }
            ofPopMatrix();
        }
    }
    ofPopView();

    // info
    stringstream ss;
    ss << "TESTING CALIBRATION" << endl;
    if (camParams.isValid()) ss << "error = " << calibrator.getReprjError();
    else                     ss << "could not obtain calibration";
    ss << endl;
    ss << (bUndistort ? "undistorted" : "distorted");
    ofDrawBitmapStringHighlight(ss.str(), 15, 20);
}


void ofApp::keyReleased(int key)
{
    switch (key)
    {
    case 'a':
        bCapture = true;
        break;

    case 't':
        bTestCalibration = !bTestCalibration;
        if (bTestCalibration)
        {
            if (camParams.isValid() || calibrator.getCalibrationResults(camParams))
            {
                projectionMatrix = ofxAruco::getProjectionMatrix(camParams);
            }
        }            
        break;

    case 's':
        saveCalibration();
        break;

    case 'u':
        bUndistort = !bUndistort;
        break;

    default:
        break;
    } 
}