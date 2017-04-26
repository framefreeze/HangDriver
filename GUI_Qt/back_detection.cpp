//
// Created by 王浩强 on 17/4/26.
//

#include "back_detection.h"
using namespace std;

void back_detection::detection(Mat &frame, Point2f &center, float &radius) {
    center = Point2f(0,0);
    radius = 0;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat fgmask;
//    blur(frame, frame, Size(5,5), Point(-1, -1));

    mog->apply(frame, fgmask);

    if(fgmask.channels() != 1){
        cvtColor(fgmask, fgmask, CV_BGR2GRAY);
    }
    threshold( fgmask, fgmask, 200, 255, THRESH_BINARY ); // 参数可继续调整
    findContours(fgmask, contours, hierarchy,  RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) ); //

    vector<vector<Point> > contours_poly( contours.size() );
    for( size_t i=0; i < contours.size(); i++){
        if(contourArea(contours[i]) > 1000){
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true);
            minEnclosingCircle( contours_poly[i], center, radius );
            circle( frame, center, (int)radius, Scalar(0,255,0), 2, 8, 0 );
            break;
        }
    }
}

void back_detection::detection_cuda(Mat &frame, Point2f &center, float &radius) {
    center = Point2f(-1,-1);
    radius = 0;
    Mat fgmask;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cuda::GpuMat cuda_frame, cuda_fgmask;
//    blur(frame, frame, Size(5,5), Point(-1, -1));
    cuda_frame.upload(frame);

    mog_cuda->apply(cuda_frame, cuda_fgmask);
    cuda_fgmask.download(fgmask);

    if(fgmask.channels() != 1){
        cvtColor(fgmask, fgmask, CV_BGR2GRAY);
    }
//        cuda::threshold(cuda_fgmask, cuda_fgmask,100, 255, THRESH_BINARY);
    threshold( fgmask, fgmask, 200, 255, THRESH_BINARY ); // 参数可继续调整
    findContours(fgmask, contours, hierarchy,  RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) ); //

    vector<vector<Point> > contours_poly( contours.size() );
    for( size_t i=0; i < contours.size(); i++){
        if(contourArea(contours[i]) > 1000){
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true);
            minEnclosingCircle( contours_poly[i], center, radius );
            circle( frame, center, (int)radius, Scalar(0,0,255), 2, 8, 0 );
            break;
        }
    }
    cuda_fgmask.release();
    cuda_frame.release();
}

void back_detection::change_pos(Point2f center) {
    int pos = 0;
    for(int i=0; i<40 ;i++){
        if(center.x >= locate[i].x && center.x <= locate[i].x+locate[i].width){
            if(center.y >= locate[i].y && center.y <= locate[i].y+locate[i].height){
                pos = i;
                break;
            }
        }
    }
    if(pos != 0){
        pos_now = pos+1;
    }
}