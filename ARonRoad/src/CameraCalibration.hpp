//
// Created by Kevin_Feng on 2017/3/1.
//

#ifndef ARONROAD_CAMERACALIBRATION_HPP
#define ARONROAD_CAMERACALIBRATION_HPP

#include <opencv2/opencv.hpp>
class CameraCalibration {
public:
    CameraCalibration();
    CameraCalibration(float fx, float fy, float cx, float cy);
    CameraCalibration(float fx, float fy, float cx, float cy, float distorsionCoeff[5]);

    void getMatrix34(float cparam[3][4]) const;

    const cv::Matx33f& getIntrinsic() const;
    const cv::Mat_<float>&  getDistorsion() const;

    float& fx();
    float& fy();

    float& cx();
    float& cy();

    float fx() const;
    float fy() const;

    float cx() const;
    float cy() const;
private:
    cv::Matx33f     m_intrinsic;
    cv::Mat_<float> m_distortion;
};


#endif //ARONROAD_CAMERACALIBRATION_HPP
