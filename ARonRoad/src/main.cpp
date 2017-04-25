//
// Created by Kevin_Feng on 2017/2/28.
//

#include <gl.h>
#include <glu.h>
#include <opencv2/opencv.hpp>
#include <cstring>
#include "ARDrawingContext.hpp"
#include "CameraCalibration.hpp"
#include "GeometryTypes.hpp"
using namespace std;
Transformation pose3d;
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture);
bool processFrame(const cv::Mat& cameraFrame, ARDrawingContext& drawingCtx);
int main(int argc, char ** argv)
{
//    CameraCalibration calibration(9.9423918882311898e+02, 9.9423918882311898e+02, 6.0137200957740095e+02, 3.3964666038906381e+02);
    CameraCalibration calibration(1.0227712011509490e+03, 1.0227712011509490e+03, 6.4474160636291322e+02, 3.6850818576406732e+02);
    cv::Mat patternImage = cv::imread(argv[1]);
//    Transformation pose3d;
    pose3d.r().mat[0][0]=0.99975646;
    pose3d.r().mat[1][0]=0.013274947;
    pose3d.r().mat[2][0]=-0.017630726;
    pose3d.r().mat[0][1]=-0.019764567;
    pose3d.r().mat[1][1]=0.18309471;
    pose3d.r().mat[2][1]=-0.98289657;
    pose3d.r().mat[0][2]=-0.0098198075;
    pose3d.r().mat[1][2]=0.98300564;
    pose3d.r().mat[2][2]=0.18331249;


    pose3d.t().data[0]=-0.073232695;
    pose3d.t().data[1]=0.74967629;
    pose3d.t().data[2]=3.7285576;
    pose3d.getInverted();
    if (patternImage.empty())
    {
        std::cout << "Input image cannot be read" << std::endl;
        return 2;
    }
    if (argc == 2)
    {
        cv::VideoCapture cap;
        cap.open(0);
        processVideo(patternImage, calibration, cap);
    }
    else
    {
        std::cerr << "Invalid number of arguments passed" << std::endl;
        return 1;
    }

    return 0;
}
void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture) {
    // Grab first frame to get the frame dimensions
    cv::Mat currentFrame;
    capture >> currentFrame;
//    cv::resize(currentFrame, currentFrame, cv::Size(640,480));

    // Check the capture succeeded:
    if (currentFrame.empty()) {
        std::cout << "Cannot open video capture device" << std::endl;
        return;
    }
    cv::Size frameSize(currentFrame.cols, currentFrame.rows);
    ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration);
    bool shouldQuit = false;
    do {

        capture >> currentFrame;
//        cv::resize(currentFrame, currentFrame, cv::Size(640,480));
//        currentFrame = currentFrame(cv::Rect(0,0,currentFrame.cols/2, currentFrame.rows));
//        cv::rotate(currentFrame, currentFrame, cv::ROTATE_180);
        if (currentFrame.empty()) {
            shouldQuit = true;
            continue;
        }

        shouldQuit = processFrame(currentFrame, drawingCtx);
    } while (!shouldQuit);
}
bool processFrame(const cv::Mat& cameraFrame, ARDrawingContext& drawingCtx)
{
    // Clone image used for background (we will draw overlay on it)

    cv::Mat img = cameraFrame.clone();

    //setTurn
    drawingCtx.setTurn();


    // Set a new camera frame:
    drawingCtx.updateBackground(img);

    // Find a pattern and update it's detection status:
//    drawingCtx.isPatternPresent = pipeline.processFrame(cameraFrame);
    drawingCtx.isPatternPresent = true;

    // Update a pattern pose:
//    drawingCtx.patternPose = pipeline.getPatternLocation();
    drawingCtx.patternPose = pose3d;

    // Request redraw of the window:
    drawingCtx.updateWindow();

    // Read the keyboard input:
    int keyCode = cv::waitKey(10);

    bool shouldQuit = false;
    if (keyCode == 27 || keyCode == 'q')
    {
        shouldQuit = true;
    }

    return shouldQuit;
}
