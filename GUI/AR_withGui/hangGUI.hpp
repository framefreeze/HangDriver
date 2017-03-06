//
// Created by Kevin_Feng on 2017/3/5.
//

#ifndef GUI_INT_HANGGUI_HPP
#define GUI_INT_HANGGUI_HPP

#include <opencv2/opencv.hpp>

class hangGUI {
public:
    void init();
    cv::Mat getImage(int,int);//得到含有五角星的图片
    cv::Mat drawPos(int, int);
    int x;//坐标
    int y;
    cv::Mat pentagram;//终点图片 五角星
    cv::Mat bgimg;//背景图片
private:
    void draw_pentagram(int,int,cv::Mat&);
};


#endif //GUI_INT_HANGGUI_HPP
