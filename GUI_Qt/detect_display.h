//
// Created by 王浩强 on 17/4/23.
//

#ifndef SSD_CPP_CLASS_DETECT_DISPLAY_H
#define SSD_CPP_CLASS_DETECT_DISPLAY_H

#include "mxnet/c_predict_api.h"
#include "back_detection.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
#include <ctime>

class detect_display {
private:
    PredictorHandle predictor_;
    std::vector<char> buffer_;
    int width_;
    int height_;
    float mean_r_;
    float mean_g_;
    float mean_b_;
    double fps;
    std::vector<std::string> class_names = {
            "aeroplane", "bicycle", "bird", "boat",
            "bottle", "bus", "car", "cat", "chair",
            "cow", "diningtable", "dog", "horse",
            "motorbike", "person", "pottedplant",
            "sheep", "sofa", "train", "tvmonitor"};
    std::vector<cv::Scalar> colors;


public:
    detect_display(int width=300, int height=300,
                   float mean_r=132.f, float mean_g=117.f, float mean_b=104.f,
                   int device_type=2, int device_id=0);

    std::vector<float> detect(cv::Mat& in_img_mat);
    std::vector<float> detect(std::string in_img) {
        cv::Mat img_mat = cv::imread(in_img);
        detect(img_mat);
    }
    std::vector<float> detect(const char *in_img) {
        return detect(std::string(in_img));
    }

    void show_result(cv::Mat src, cv::Mat& dst, std::vector<float> &dets,
                     float visu_thresh);
    void show_result(std::string filename, cv::Mat& dst, std::vector<float> &dets,
                     float visu_thresh) {
        cv::Mat src = cv::imread(filename);
        show_result(src, dst, dets, visu_thresh);
    }

protected:
    void GetMeanFile(cv::Mat image_file, mx_float* image_data,
                     const int channels, const cv::Size resize_size);

    void GetMeanFile(const std::string image_file, mx_float* image_data,
                const int channels, const cv::Size resize_size) {
        cv::Mat img_mat = cv::imread(image_file);
        GetMeanFile(img_mat, image_data, channels, resize_size);
    }

};

#endif //SSD_CPP_CLASS_DETECT_DISPLAY_H
