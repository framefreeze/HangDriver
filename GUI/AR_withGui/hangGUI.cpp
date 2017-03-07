//
// Created by Kevin_Feng on 2017/3/5.
//

#include "hangGUI.hpp"
#include <iostream>
#include <stdio.h>
using namespace cv;
Rect locate2[41] = {Rect(61, 0, 64, 93), Rect(125, 0, 62, 93), Rect(61, 93, 64, 118),
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

//void detect_Mouse(int event,int x,int y,int flags,void *param);
int check_locate2(int x, int y);
void hangGUI::init() {
    Mat tmp = imread("/home/framefreeze/Documents/HangDriver/GUI/fgps.png");
    resize(tmp, tmp, Size(512, 512));
    bgimg = cv::Mat::zeros(tmp.rows, tmp.cols+350, tmp.type());//初始化背景
    Mat tmp2 = bgimg(Rect(0,0,tmp.cols, tmp.rows));
    addWeighted(tmp2,0.1,tmp,0.9,0.0,tmp2);
    std::cout << bgimg.size() << std::endl;

    if(bgimg.empty()){
        printf("error");
        return ;
    }
    this->pentagram = imread("/home/framefreeze/Documents/HangDriver/GUI/pentagram.png");//初始化目的地图标
    resize(pentagram,pentagram,Size(32,32));
//    resize(bgimg, bgimg, Size(512, 512));



//    print()
    waitKey();
}
void hangGUI::draw_pentagram(int x, int y, Mat& img) {

}
Mat hangGUI::getImage(int x, int y, int& dst){
    Mat frame;
    bgimg.copyTo(frame);
    Rect roi(x-pentagram.cols/2, y-pentagram.rows/2, pentagram.cols, pentagram.rows);
//    frame(roi) = pentagram;
    Mat tmp = frame(roi);
    addWeighted(tmp, 0.5 ,pentagram,0.5,0.0,tmp);

    dst = check_locate2(373, 213);
    char pos_str[100];
    sprintf(pos_str,"Set distination to %d!", dst);
    cv::putText(frame,pos_str, cv::Point2f(512,30), FONT_HERSHEY_SIMPLEX , 1, Scalar(255,255,255), 3);
    bgimg = frame;
    return frame;

}
Mat hangGUI::drawPos(int x, int y, int pos, int turn) {
    Mat frame;
    bgimg.copyTo(frame);

    char pos_str[100], turn_str[100];
    sprintf(pos_str,"Real pos is in %d!",pos);
//    if (x != -2){
        x = locate2[pos-1].x + locate2[pos-1].width/2;
        y = locate2[pos-1].x + locate2[pos-1].width/2;
        x = x*430/478 + 30;
        y = y*512/570;
//    }
//    circle(frame, Point(x,y), 5, Scalar(0,255,0),3);
    cv::putText(frame,pos_str, cv::Point2f(512,60), FONT_HERSHEY_SIMPLEX , 1, Scalar(255,255,255), 3);

//    if(turn == -1){
//        sprintf(turn_str, "Arrival!");
//    }
//    else if(turn == 1){
//        sprintf(turn_str, "Go straight!");
//    }
//    else if(turn == 2){
//        sprintf(turn_str, "Arr");
//    }
    switch(turn){
        case -1 :{
            sprintf(turn_str, "Arrival!");
            break;
        }
        case 1 :{
            sprintf(turn_str, "Go straight!");
            break;
        }
        case 2 :{
            sprintf(turn_str, "Turn left!");
            break;
        }
        case 3 :{
            sprintf(turn_str, "Turn right!");
            break;
        }
    }
    cv::putText(frame, turn_str, cv::Point2f(512,90), FONT_HERSHEY_SIMPLEX , 1, Scalar(255,255,255), 3);

    return frame;
}

int check_locate2(int x, int y){
    int pos_dst = -1;
    for (int i=0; i<40; i++){
        if(x >= locate2[i].x && x <= locate2[i].x+locate2[i].width){
            if(y >= locate2[i].y && y <= locate2[i].y+locate2[i].height){
                pos_dst = i+1;
                break;
            }
        }
    }
    return pos_dst;
}