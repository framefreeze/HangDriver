#include<cv.h>
#include<cxcore.h>
#include<highgui.h>
#include<opencv2/opencv.hpp>
#include<vector>
#include<cstdio>
#include<iostream>
using namespace std;
using namespace cv;
void drawDetectLines(Mat& image,const vector<Vec4i>& lines,Scalar & color)
{
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it=lines.begin();
    while(it!=lines.end())
    {
        Point pt1((*it)[0],(*it)[1]);
        Point pt2((*it)[2],(*it)[3]);
        line(image,pt1,pt2,color,2); //  线条宽度设置为2
        ++it;
    }
}

int main(){
    //声明IplImage指针
    char filename[200] = "/Users/yujifan/Documents/projects/project2017/data/IMG_0906.MOV";
    IplImage* pFrame = NULL;
    IplImage* pCutFrame = NULL;
    IplImage* pCutFrImg = NULL;
    IplImage* pCutBkImg = NULL;
    //声明CvMat指针
    CvMat* pCutFrameMat = NULL;
    CvMat* pCutFrMat = NULL;
    CvMat* pCutBkMat = NULL;
    //声明CvCapture指针
    CvCapture* pCapture = NULL;
    //声明CvMemStorage和CvSeg指针
    CvMemStorage* storage = cvCreateMemStorage();
    CvSeq* lines = NULL;
    //当前帧数
    int nFrmNum = 0;
    //裁剪的天空高度
    int CutHeight = 700;
    //窗口命名
    cvNamedWindow("video", 1);
    //cvNamedWindow("background", 1);
    cvNamedWindow("foreground", 1);
    //调整窗口初始位置
    cvMoveWindow("video", 300, 30);
    cvMoveWindow("background", 100, 100);
    cvMoveWindow("foreground", 300, 370);
    //不能打开则退出
    if (!(pCapture = cvCaptureFromFile(filename))){
        fprintf(stderr, "Can not open video file\n");
        return -2;
    }
    //每次读取一桢的视频
    while (pFrame = cvQueryFrame(pCapture)){
        //设置ROI裁剪图像
        cvSetImageROI(pFrame, cvRect((pFrame->width)*2/9, CutHeight, (pFrame->width)/2, pFrame->height - CutHeight-50));
        nFrmNum++;
        //第一次要申请内存p
        if (nFrmNum == 1){
            pCutFrame = cvCreateImage(cvSize((pFrame->width)/2, pFrame->height - CutHeight-50), pFrame->depth, pFrame->nChannels);
            cvCopy(pFrame, pCutFrame, 0);
            pCutBkImg = cvCreateImage(cvSize((pFrame->width)/2, pFrame->height - CutHeight-50), IPL_DEPTH_8U, 1);
            pCutFrImg = cvCreateImage(cvSize((pFrame->width)/2, pFrame->height - CutHeight-50), IPL_DEPTH_8U, 1);

            pCutBkMat = cvCreateMat(pCutFrame->height, pCutFrame->width, CV_32FC1);
            pCutFrMat = cvCreateMat(pCutFrame->height, pCutFrame->width, CV_32FC1);
            pCutFrameMat = cvCreateMat(pCutFrame->height, pCutFrame->width, CV_32FC1);
            //转化成单通道图像再处理
            cvCvtColor(pCutFrame, pCutBkImg, CV_BGR2GRAY);
            cvCvtColor(pCutFrame, pCutFrImg, CV_BGR2GRAY);
            //转换成矩阵
            cvConvert(pCutFrImg, pCutFrameMat);
            cvConvert(pCutFrImg, pCutFrMat);
            cvConvert(pCutFrImg, pCutBkMat);
        }
        else{
            //获得剪切图
            cvCopy(pFrame, pCutFrame, 0);
            //前景图转换为灰度图
            cvCvtColor(pCutFrame, pCutFrImg, CV_BGR2GRAY);
            cvConvert(pCutFrImg, pCutFrameMat);
            //高斯滤波先，以平滑图像
            cvSmooth(pCutFrameMat, pCutFrameMat, CV_GAUSSIAN, 3, 0, 0.0);
            //当前帧跟背景图相减
            cvAbsDiff(pCutFrameMat, pCutBkMat, pCutFrMat);
            //二值化前景图
            cvThreshold(pCutFrMat, pCutFrImg, 35, 255.0, CV_THRESH_BINARY);
            //进行形态学滤波，去掉噪音
            cvErode(pCutFrImg, pCutFrImg, 0, 1);
            cvDilate(pCutFrImg, pCutFrImg, 0, 1);
            //更新背景
            cvRunningAvg(pCutFrameMat, pCutBkMat, 0.003, 0);
            //pCutBkMat = cvCloneMat(pCutFrameMat);
            //将背景转化为图像格式，用以显示
            //cvConvert(pCutBkMat, pCutBkImg);
            cvCvtColor(pCutFrame, pCutBkImg, CV_BGR2GRAY);
            //canny变化
            cvCanny(pCutFrImg, pCutFrImg, 50, 100);
            lines = cvHoughLines2(pCutFrImg, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 100, 30, 15);
            printf("Lines number: %d\n", lines->total);
            //画出直线
            for (int i = 0; i<lines->total; i++){
                CvPoint* line = (CvPoint* )cvGetSeqElem(lines, i);
                cvLine(pFrame, line[0], line[1], CV_RGB(255, 0, 0), 6, CV_AA);
            }
            //显示图像
            cvResetImageROI(pFrame);
            cvShowImage("video", pFrame);
            cvShowImage("background", pCutBkImg);
            cvShowImage("foreground", pCutFrImg);
            //按键事件，空格暂停，其他跳出循环
            int temp = cvWaitKey(2);
            if (temp == 32){
                while (cvWaitKey() == -1);
            }
            else if (temp >= 0){
                break;
            }
        }
        //恢复ROI区域（多余可去掉）
//        cvResetImageROI(pFrame);
    }
    //销毁窗口
    cvDestroyWindow("video");
    cvDestroyWindow("background");
    cvDestroyWindow("foreground");
    //释放图像和矩阵
    cvReleaseImage(&pCutFrImg);
    cvReleaseImage(&pCutBkImg);
    cvReleaseImage(&pCutFrame);
    cvReleaseMat(&pCutFrameMat);
    cvReleaseMat(&pCutFrMat);
    cvReleaseMat(&pCutBkMat);
    cvReleaseCapture(&pCapture);

    return 0;
}