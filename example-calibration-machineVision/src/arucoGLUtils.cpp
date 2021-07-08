#include "arucoGLUtils.h"
#include "glm/gtc/type_ptr.hpp"
#include "opencv2/calib3d/calib3d.hpp"


glm::mat4 ofxAruco::getProjectionMatrix(aruco::CameraParameters& camParams, float nearClip, float farClip)
{
    double matrix[16];
    camParams.glGetProjectionMatrix(camParams.CamSize, camParams.CamSize, matrix, nearClip, farClip, true);
    return glm::make_mat4(matrix);
}


glm::mat4 ofxAruco::getModelViewMatrix(aruco::Marker& marker)
{
    double matrix[16];
    if (marker.isPoseValid()) marker.glGetModelViewMatrix(matrix);
    return glm::make_mat4(matrix);
}


glm::mat4 ofxAruco::getModelViewMatrix(const aruco::MarkerMapPoseTracker& mapPoseTracker)
{
    const cv::Mat& Rvec = mapPoseTracker.getRvec();
    const cv::Mat& Tvec = mapPoseTracker.getTvec();

    cv::Mat Rot(3, 3, CV_32FC1);
    cv::Rodrigues(Rvec, Rot);

    double para[3][4];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            para[i][j] = Rot.at<float>(i, j);
        }
    }
    // now, add the translation
    para[0][3] = Tvec.ptr<float>(0)[0];
    para[1][3] = Tvec.ptr<float>(0)[1];
    para[2][3] = Tvec.ptr<float>(0)[2];
    double scale = 1;

    double matrix[16];
    matrix[0 + 0 * 4] = para[0][0];
    // R1C2
    matrix[0 + 1 * 4] = para[0][1];
    matrix[0 + 2 * 4] = para[0][2];
    matrix[0 + 3 * 4] = para[0][3];
    // R2
    matrix[1 + 0 * 4] = para[1][0];
    matrix[1 + 1 * 4] = para[1][1];
    matrix[1 + 2 * 4] = para[1][2];
    matrix[1 + 3 * 4] = para[1][3];
    // R3
    matrix[2 + 0 * 4] = -para[2][0];
    matrix[2 + 1 * 4] = -para[2][1];
    matrix[2 + 2 * 4] = -para[2][2];
    matrix[2 + 3 * 4] = -para[2][3];
    matrix[3 + 0 * 4] = 0.0;
    matrix[3 + 1 * 4] = 0.0;
    matrix[3 + 2 * 4] = 0.0;
    matrix[3 + 3 * 4] = 1.0;
    if (scale != 0.0)
    {
        matrix[12] *= scale;
        matrix[13] *= scale;
        matrix[14] *= scale;
    }

    return glm::make_mat4(matrix);
}