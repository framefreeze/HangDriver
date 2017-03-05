//
// Created by Kevin_Feng on 2017/3/5.
//

#include "hangGUI.hpp"

void detect_Mouse(int event,int x,int y,int flags,void *param);
int main(){
    hangGUI hanggui;
    hanggui.init();

}
void detect_Mouse(int event,int x,int y,int flags,void *param) {
    if (event == CV_EVENT_LBUTTONUP) {
        printf("x: %d,y: %d\n", x, y);
    }
    //rectangle()

}