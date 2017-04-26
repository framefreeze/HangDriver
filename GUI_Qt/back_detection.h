//
// Created by 王浩强 on 17/4/26.
//

#ifndef SSD_CPP_CLASS_BACK_DETECTION_H
#define SSD_CPP_CLASS_BACK_DETECTION_H
#include <opencv2/opencv.hpp>
#include <opencv2/cudalegacy.hpp>
//#include <cuda_runtime.h>
using namespace cv;

class back_detection {
private:
    Ptr<BackgroundSubtractor> mog_cuda = cuda::createBackgroundSubtractorMOG2();
    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
    Rect locate[41] = {Rect(61, 0, 64, 93), Rect(125, 0, 62, 93), Rect(61, 93, 64, 118),
                       Rect(125, 93, 62, 118), Rect(61,211,64,91), Rect(125,211,62,91),
                       Rect(61,301,64,94), Rect(125,301,62,94), Rect(61,395,64,120),
                       Rect(125,395,62,120), Rect(61,516,64,54), Rect(125,516,62,54),
                       Rect(188,395,92,60), Rect(188,455,92,61), Rect(280,395,90,60),
                       Rect(280,455,90,61), Rect(370,0,63,93), Rect(432,0,63,93),
                       Rect(370,93,126,118), Rect(370,211,63,91), Rect(432,211,63,91),
                       Rect(370,301,63,94), Rect(432,301,63,94), Rect(370,395,63,60),
                       Rect(370,455,63,61), Rect(432, 395,63,120), Rect(495, 93,52,61),
                       Rect(495,154,52,57), Rect(188,93,92,61), Rect(280,93,90,61),
                       Rect(188,154,92,57), Rect(280, 154,90,57), Rect(0,0,60,93),
                       Rect(0,93,60,118), Rect(0,211,60,91), Rect(0,301,60,94),
                       Rect(0,395,60,120), Rect(0,516,60,54), Rect(0,0,0,0),
                       Rect(203,260,135,120)};
    int pos_now;
public:
    back_detection();
    void detection_cuda(Mat& frame, Point2f& center, float& radius );
    void detection(Mat& frame, Point2f& center, float& radius );
    void change_pos(Point2f center);
    int get_pos();
};


#endif //SSD_CPP_CLASS_BACK_DETECTION_H
