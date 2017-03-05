//
// Created by Kevin_Feng on 2017/3/5.
//

#include "hangGUI.hpp"
using namespace cv;

void detect_Mouse(int event,int x,int y,int flags,void *param);
void hangGUI::init() {
    this->bgimg = imread("/home/framefreeze/Documents/HangDriver/GUI/fgps.png");//初始化背景
    if(bgimg.empty()){
        printf("error");
       return ;
    }
    this->pentagram = imread("/home/framefreeze/Documents/HangDriver/GUI/pentagram.png");//初始化目的地图标
    resize(pentagram,pentagram,Size(32,32));
    resize(bgimg, bgimg, Size(512, 512));



//    print()
    waitKey();
}
void hangGUI::draw_pentagram(int x, int y, Mat& img) {

}
Mat hangGUI::getImage(int x, int y){
    Mat frame;
    bgimg.copyTo(frame);
    Rect roi(x-pentagram.cols/2, y-pentagram.rows/2, pentagram.cols, pentagram.rows);
//    frame(roi) = pentagram;
    Mat tmp = frame(roi);
    addWeighted(tmp, 0.5 ,pentagram,0.5,0.0,tmp);
    return frame;

}
Mat hangGUI::drawPos(int x, int y) {
    Mat frame;
    bgimg.copyTo(frame);
    circle(frame, Point(x,y), 5, Scalar(0,255,0),3);

    return frame;
}