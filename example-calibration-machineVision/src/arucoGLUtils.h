#pragma once

#include "aruco.h"
#include "glm/glm.hpp"


namespace ofxAruco
{
    glm::mat4 getProjectionMatrix(aruco::CameraParameters& camParams, float nearClip = .1f, float farClip = 5.f);

    glm::mat4 getModelViewMatrix(aruco::Marker& marker);

    glm::mat4 getModelViewMatrix(const aruco::MarkerMapPoseTracker& mapPoseTracker);

    //glm::mat4 getMapModelViewMatrix(const cv::Mat& Rvec, const cv::Mat& Tvec);
}