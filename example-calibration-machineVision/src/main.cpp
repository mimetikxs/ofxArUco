#include "ofApp.h"

int main(){
    ofGLWindowSettings settings;
    settings.setSize(640, 480);
    settings.windowMode = OF_WINDOW;
    settings.setGLVersion(3, 1); // programmable renderer

    auto mainWindow = ofCreateWindow(settings);
    auto mainApp = make_shared<ofApp>();

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
