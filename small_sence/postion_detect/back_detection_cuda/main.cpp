#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/cudalegacy.hpp>
#include "output_road.cpp"

/********AR********/
#include "AR/ARDrawingContext.hpp"
#include "AR/ARPipeline.hpp"
#include "AR/DebugHelpers.hpp"

//#include "opencv2/cudabgsegm.hpp"
//#include <cuda_runtime.h>

using namespace std;
using namespace cv;
//using namespace cv::cuda;
Ptr<BackgroundSubtractor> mog_cuda = cuda::createBackgroundSubtractorMOG2();
Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
Mat frame;
/************AR***********/
bool processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx);
Mat ARFrame;

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
int main() {


    //char videoname[100] = "./best.webm";
    //VideoCapture video(videoname);
    VideoCapture video(0);
    VideoCapture ARVideo("./AR/output_left.mp4");
    if(!video.isOpened()) return 1;


    Point2f center(0,0);
    Point2f center2[10] = {Point2f(0,0)};

    int pos_real  = 0;
    float radius = 0;
    bool change = true;
    int y;


    /***********AR*********/
    int turn; //转向
    CameraCalibration calibration(1040.4711914092852f, 1042.1274843132999f, 542.12340728399488f, 362.22154256297267f);
    Mat patternImage = imread("");



    while(true) {
        if(change){
            printf("Please input your destination:" );
            scanf("%d", &y);
            change = false;
        }
        video >> frame;
        ARVideo >> ARFrame;
        if( frame.empty() ){
            break;
        }
       
        frame = frame(Rect(70,0,frame.cols-70,frame.rows));
        rotate(frame, frame, ROTATE_90_COUNTERCLOCKWISE);
//         cout << frame.cols << frame.rows << endl;
        detection(frame, center, radius);
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
            pos_real = pos+1;
            //printf("%d\n", pos_real);
            //output_route_with_direction(pos_real, y, change);
            turn = direct(pos_real,y);
        }

        /******hua chu AR*****/
        Size frameSize(ARFrame.cols, ARFrame.rows);


        ARPipeline pipeline(patternImage, calibration);
        ARDrawingContext drawingCtx("Markerless AR", frameSize, calibration,turn);

        processFrame(ARFrame,pipeline,drawingCtx);

        /*
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
        */
        imshow("frame", frame);

        if(waitKey(30) == 27) {
            break;
        }

    }

    return 0;
}

bool processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx)
{
    // Clone image used for background (we will draw overlay on it)
    cv::Mat img = cameraFrame.clone();

    // Draw information:
    if (pipeline.m_patternDetector.enableHomographyRefinement)
        cv::putText(img, "Pose refinement: On   ('h' to switch off)", cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));
    else
        cv::putText(img, "Pose refinement: Off  ('h' to switch on)",  cv::Point(10,15), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    cv::putText(img, "RANSAC threshold: " + ToString(pipeline.m_patternDetector.homographyReprojectionThreshold) + "( Use'-'/'+' to adjust)", cv::Point(10, 30), CV_FONT_HERSHEY_PLAIN, 1, CV_RGB(0,200,0));

    // Set a new camera frame:
    drawingCtx.updateBackground(img);

    // Find a pattern and update it's detection status:
    drawingCtx.isPatternPresent = pipeline.processFrame(cameraFrame);

    // Update a pattern pose:
    drawingCtx.patternPose = pipeline.getPatternLocation();

    // Request redraw of the window:
    drawingCtx.updateWindow();

    // Read the keyboard input:
    int keyCode = cv::waitKey(5);

    bool shouldQuit = false;
    if (keyCode == '+' || keyCode == '=')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold += 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::min(10.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == '-')
    {
        pipeline.m_patternDetector.homographyReprojectionThreshold -= 0.2f;
        pipeline.m_patternDetector.homographyReprojectionThreshold = std::max(0.0f, pipeline.m_patternDetector.homographyReprojectionThreshold);
    }
    else if (keyCode == 'h')
    {
        pipeline.m_patternDetector.enableHomographyRefinement = !pipeline.m_patternDetector.enableHomographyRefinement;
    }
    else if (keyCode == 27 || keyCode == 'q')
    {
        shouldQuit = true;
    }

    return shouldQuit;
}