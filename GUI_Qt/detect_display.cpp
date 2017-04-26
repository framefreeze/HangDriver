//
// Created by 王浩强 on 17/4/23.
//
#define APPLE 1
#include "detect_display.h"
#include <chrono>

using namespace cv;
using namespace std::chrono;

// Read file to buffer
class BufferFile {
public :
    std::string file_path_;
    int length_;
    char* buffer_;

    explicit BufferFile(std::string file_path)
            :file_path_(file_path) {

        std::ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary);
        if (!ifs) {
            std::cerr << "Can't open the file. Please check " << file_path << ". \n";
            length_ = 0;
            buffer_ = NULL;
            return;
        }

        ifs.seekg(0, std::ios::end);
        length_ = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        std::cout << file_path.c_str() << " ... "<< length_ << " bytes\n";

        buffer_ = new char[sizeof(char) * length_];
        ifs.read(buffer_, length_);
        ifs.close();
    }

    int GetLength() {
        return length_;
    }
    char* GetBuffer() {
        return buffer_;
    }

    ~BufferFile() {
        if (buffer_) {
            delete[] buffer_;
            buffer_ = NULL;
        }
    }
};

void detect_display::GetMeanFile(cv::Mat image_mat, mx_float* image_data,
                 const int channels, const cv::Size resize_size)
{
    cv::Mat im;
    cv::resize(image_mat, im, resize_size);

    int size = im.rows * im.cols * channels;

    mx_float* ptr_image_r = image_data;
    mx_float* ptr_image_g = image_data + size / 3;
    mx_float* ptr_image_b = image_data + size / 3 * 2;

    for (int i = 0; i < im.rows; i++) {
        uchar* data = im.ptr<uchar>(i);

        for (int j = 0; j < im.cols; j++) {
            if (channels > 1)
            {
                mx_float b = static_cast<mx_float>(*data++) - mean_b_;
                mx_float g = static_cast<mx_float>(*data++) - mean_g_;
                *ptr_image_g++ = g;
                *ptr_image_b++ = b;
            }

            mx_float r = static_cast<mx_float>(*data++) - mean_r_;
            *ptr_image_r++ = r;

        }
    }
}

detect_display::detect_display(int width, int height, float mean_r, float mean_g,
                               float mean_b, int device_type, int device_id) {
    int tmp_r,tmp_g,tmp_b;
    width_ = width;
    height_ = height;
    const char *input_name = "data";
    const char *input_keys[1];
    input_keys[0] = input_name;
    const mx_uint input_shape_indptr[] = {0, 4};
    const mx_uint input_shape_data[] = {1, 3, static_cast<mx_uint>(width_), static_cast<mx_uint>(height_)};
    mean_r_ = mean_r;
    mean_g_ = mean_g;
    mean_b_ = mean_b;

#ifdef APPLE
    std::string json_file = "/Users/DavidWang/Documents/learn/mxnet-ssd/model/deploy_ssd_300-symbol.json";
    std::string param_file = "/Users/DavidWang/Documents/learn/mxnet-ssd/model/deploy_ssd_300-0000.params";
#else
    std::string json_file = "/home/framefreeze/Documents/HangDriver/GUI_Qt/data/deploy_ssd_300-symbol.json";
    std::string param_file = "/home/framefreeze/Documents/HangDriver/GUI_Qt/data/deploy_ssd_300-0001.params";
#endif

    BufferFile json_data(json_file);
    BufferFile param_data(param_file);


#ifdef APPLE
    device_type = 1;
#endif

    // Create Predictor
    MXPredCreate((const char*)json_data.GetBuffer(),
                 (const char*)param_data.GetBuffer(),
                 static_cast<size_t>(param_data.GetLength()),
                 device_type, device_id, 1,
                 input_keys, input_shape_indptr, input_shape_data,
                 &predictor_);
    if(predictor_ == NULL){
        std::cout << "init_error" << std::endl;
    }

    for(int i = 0; i < class_names.size(); i++) {
        std::srand(std::time(0));
        tmp_r = std::rand() % 255;
        tmp_g = std::rand() % 255;
        tmp_b = std::rand() % 255;
        colors.push_back(Scalar(tmp_r, tmp_g, tmp_b));
    }
}

std::vector<float> detect_display::detect(Mat& img) {

    // Just a big enough memory 1000x1000x3
    int image_size = width_ * height_ * 3;

    std::vector<mx_float> image_data = std::vector<mx_float>(image_size);

    //-- Read Mean Data
    GetMeanFile(img, image_data.data(), 3, cv::Size(width_, height_));

    //-- Set Input Image
    MXPredSetInput(predictor_, "data", image_data.data(), image_size);

    high_resolution_clock ::time_point t2 = high_resolution_clock ::now();
//    std::cout << "It took me " << time_span.count() << " seconds." << std::endl;

    // use model to forward
    mx_uint *shape = NULL;
    mx_uint shape_len = 0;

    MXPredForward(predictor_);
    MXPredGetOutputShape(predictor_, 0, &shape, &shape_len);
    mx_uint tt_size = 1;
    for (mx_uint i = 0; i < shape_len; ++i) {
        tt_size *= shape[i];
    }
    assert(tt_size % 6 == 0);
    std::vector<float> outputs(tt_size);
    MXPredGetOutput(predictor_, 0, outputs.data(), tt_size);

    high_resolution_clock ::time_point t3 = high_resolution_clock ::now();
    duration<double> time_spend = duration_cast<duration<double>>(t3 - t2);
    fps = 1.0f / time_spend.count();
//    std::cout << "Predict took me " << time_span2.count() << " seconds.";

    return outputs;
}

void detect_display::show_result(cv::Mat src, cv::Mat &dst, std::vector<float> &dets,
                                 float visu_thresh) {
    dst = src.clone();
    int width = src.cols;
    int height = src.rows;
    int baseline, bx, by;
    Mat roi, box;


    // id, score, xmin, ymin, xmax, ymax
    assert (dets.size() % 6 == 0);
    for (std::size_t i = 0; i < dets.size(); i += 6) {
        if (dets[i] < 0) continue;  // not an object
        int id = static_cast<int>(dets[i]);
        float score = dets[i + 1];
        if (score < visu_thresh) continue;
        int xmin = static_cast<int>(dets[i + 2] * width);
        int ymin = static_cast<int>(dets[i + 3] * height);
        int xmax = static_cast<int>(dets[i + 4] * width);
        int ymax = static_cast<int>(dets[i + 5] * height);

        rectangle(dst, cv::Point2f(xmin,ymin), cv::Point2f(xmax, ymax), colors[id], 5);

//        std::string display_str = format("Type: %s, rate: %lf", class_names[id].c_str(), score);
        std::string display_str = format("%s %.3lf", class_names[id].c_str(), score);
        Size textSize = getTextSize(display_str, 0, 1, 1, &baseline);

        // draw the box
        bx = (xmin + textSize.width) > width ? (width-xmin-1) : textSize.width ;
        by = (10 + ymin + textSize.height) > height ? (height-ymin-1) : (10 + textSize.height);
        Rect text_box(xmin, ymin, bx, by);
        roi = dst(text_box);
        box = roi.clone();
        rectangle(box, Rect(0, 0, box.cols, box.rows), colors[id], -1);
        addWeighted(roi, 0.5, box, 0.5, 1, roi);

        putText(dst, display_str, Point2f(xmin, ymin+textSize.height), 0, 1, Scalar(255,255,255), 1, LINE_4);
//        std::cout << "type is: " << class_names[id] << " " << id << " rasto is: " << score << " fps: " << fps << std::endl;
    }
    putText(dst, format("fps: %.0lf", fps), Point2f(0, 30), 0, 1, Scalar(0,0,0),2);
//    imshow("reslut", dst);
//    waitKey();
}
