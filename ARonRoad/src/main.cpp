//
// Created by Kevin_Feng on 2017/2/28.
//

#include <gl.h>
#include <glu.h>
#include <opencv2/opencv.hpp>
#include <cstring>
using namespace std;
int main(int argc, char ** argv)
{
    string windowName = "hello";
    int windowHeight = 720;
    int windowWidth = 1280;
    cv::namedWindow(windowName, cv::WINDOW_OPENGL);
    // Resize it exactly to video size
    cv::resizeWindow(windowName, windowWidth, windowHeight);
    // Initialize OpenGL draw callback:
    cv::setOpenGlContext(windowName);
    cv::setOpenGlDrawCallback(windowName, ARDrawingContextDrawCallback, this);
}