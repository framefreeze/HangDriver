//
// Created by Kevin_Feng on 2017/3/5.
//

#ifndef GUI_INT_HANGGUI_HPP
#define GUI_INT_HANGGUI_HPP

#include <opencv2/opencv.hpp>

class hangGUI {
public:
    void init();
    cv::Mat getImage(int,int);
    int x;
    int y;
    cv::Mat pentagram;
    cv::Mat bgimg;
private:
    void draw_pentagram(int,int,cv::Mat&);
};


#endif //GUI_INT_HANGGUI_HPP
