//
// Created by Kevin_Feng on 2017/3/5.
//

#include "hangGUI.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;
void detect_Mouse(int event,int x,int y,int flags,void *param);
int win_x, win_y;
bool is_update = false;

int main(){
    hangGUI hanggui;
    hanggui.init();
    imshow("background", hanggui.bgimg);//显示背景
    setMouseCallback("background",detect_Mouse);//鼠标状态监测
    while(true){
        if (waitKey(10) == 27) {
            break;
        }
        if(is_update){
            Mat img = hanggui.getImage(win_x, win_y);
            imshow("background", img);
        }
    }
    return 0;
}

void detect_Mouse(int event,int x,int y,int flags,void *param) {
    if (event == CV_EVENT_LBUTTONUP) {//鼠标点击需要加强robust
        printf("x: %d,y: %d\n", x, y);
        win_x = x;
        win_y = y;
        is_update = true;
    }

    //rectangle()

}