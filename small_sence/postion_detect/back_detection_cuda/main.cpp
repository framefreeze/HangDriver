#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/cudalegacy.hpp>
//#include "opencv2/cudabgsegm.hpp"
//#include <cuda_runtime.h>

using namespace std;
using namespace cv;
//using namespace cv::cuda;
Ptr<BackgroundSubtractor> mog_cuda = cuda::createBackgroundSubtractorMOG2();
Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
Mat frame;


Point2f detection(Mat& frame, Point2f& center, float& radius ){
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
//        cuda::threshold(cuda_fgmask, cuda_fgmask,100, 255, THRESH_BINARY);
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
    return center;
}

void detection_cuda(Mat& frame, Point2f& center, float& radius ){
    center = Point2f(0,0);
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
            circle( frame, center, (int)radius, Scalar(0,255,0), 2, 8, 0 );
            break;
        }
    }
    cuda_fgmask.release();
    cuda_frame.release();
}
int main() {


    char videoname[100] = "./best.webm";
    VideoCapture video(videoname);
    if(!video.isOpened()) return 1;


    Point2f center(0,0);
    float radius = 0;
    while(true) {
        video >> frame;

        if( frame.empty() ){
            break;
        }
        detection_cuda(frame, center, radius);
        Rect roi_rect(int(center.x-radius), int(center.y-radius), int(radius*2), int(radius*2));
        //cout << roi_rect << endl;
        if(radius != 0) {
            if(roi_rect.x + roi_rect.width >= frame.cols){
                roi_rect.width = frame.cols-20-roi_rect.x;
            }
            if(roi_rect.y + roi_rect.height >= frame.rows){
                roi_rect.height = frame.rows-20-roi_rect.y;
            }
            Mat Roi = frame(roi_rect);
            //cout << Roi << endl;
            imshow("roi", Roi);
        }
        imshow("frame", frame);

        if(waitKey() == 27) {
            break;
        }

    }

    return 0;
}
