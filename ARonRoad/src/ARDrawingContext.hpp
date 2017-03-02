//
// Created by Kevin_Feng on 2017/3/1.
//

#ifndef ARONROAD_ARDRAWINGCONTEXT_H
#define ARONROAD_ARDRAWINGCONTEXT_H

#include <opencv2/opencv.hpp>
void ARDrawingContextDrawCallback(void* param);

class ARDrawingContext
{
public:
    ARDrawingContext(std::string windowName, cv::Size frameSize, const CameraCalibration& c);
    ~ARDrawingContext();


    bool                isPatternPresent;
    Transformation      patternPose;


    //! Set the new frame for the background
    void updateBackground(const cv::Mat& frame);

    void updateWindow();

private:
    friend void ARDrawingContextDrawCallback(void* param);
    //! Render entire scene in the OpenGl window
    void draw();

    //! Draws the background with video
    void drawCameraFrame();

    //! Draws the AR
    void drawAugmentedScene();

    //! Builds the right projection matrix from the camera calibration for AR
    void buildProjectionMatrix(const CameraCalibration& calibration, int w, int h, Matrix44& result);

    //! Draws the coordinate axis
    void drawCoordinateAxis();

    //! Draw the cube model
    void drawCubeModel();

    //! Draw the Arrow
    void drawArrow(float,float,float,float);

private:
    bool               m_isTextureInitialized;
    unsigned int       m_backgroundTextureId;
    CameraCalibration  m_calibration;
    cv::Mat            m_backgroundImage;
    std::string        m_windowName;
};

#endif //ARONROAD_ARDRAWINGCONTEXT_H