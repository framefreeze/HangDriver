//
// Created by Kevin_Feng on 2017/3/5.
//

#include "hangGUI.hpp"
using namespace cv;

void detect_Mouse(int event,int x,int y,int flags,void *param);
void hangGUI::init() {
    this->bgimg = imread("/Users/Kevin/Documents/Projects/HangDriver/GUI/fgps.png");//初始化背景
    if(bgimg.empty()){
        printf("error");
       return ;
    }
    this->pentagram = imread("/Users/Kevin/Documents/Projects/HangDriver/GUI/pentagram.png");//初始化目的地图标
    resize(bgimg, bgimg, Size(512, 512));

    imshow("background", bgimg);//显示背景
    setMouseCallback("background",detect_Mouse);//鼠标状态监测

//    print()
    waitKey();
}
void hangGUI::draw_pentagram(int x, int y, Mat& img) {

}
Mat hangGUI::getImage(int x, int y){
    Mat frame;
    bgimg.copyTo(frame);

    return frame;
}