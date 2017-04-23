/*!
 *  Copyright (c) 2015 by Joshua Zhang
 * \file detector.hpp
 * \brief ssd detection module impl
 */
#include <opencv2/opencv.hpp>

#include "zupply.hpp"
#include "CImg.h"
#include "detector.hpp"
#include <iostream>
#include <sstream>
#include <streambuf>
#include <cassert>
#include <cstdlib>
#include <ctime>

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/contrib/contrib.hpp>



using namespace cimg_library;
using namespace zz;
using namespace cv;

namespace det {
std::vector<std::string> load_class_map(std::string map_file) {
  std::vector<std::string> classes;
  fs::FileReader fr(map_file);
  if (!fr.is_open()) {
    auto logger = log::get_logger("default");
    logger->error("Can't open file: ") << map_file << " to read.";
    return classes;
  }
  std::string line = fr.next_line(true);
  while (!line.empty()) {
    classes.push_back(line);
    line = fr.next_line(true);
  }
  return classes;
}

void cimg_draw_box(CImg<unsigned char> &img, int x0, int y0, int x1, int y1,
                   const unsigned char *color, int thickness = 1) {
  // wrapper for drawing lines
  // first check width and height
  assert (img.spectrum() == 3);
  int width = img.width();
  int height = img.height();

  // handle thickness
  int inside = 0;
  int outside = 0;
  if (thickness > 1) {
    outside = (thickness - 1) / 2;
    inside = thickness - 1 - outside;
  }

  for (int i = -outside; i < inside + 1; ++i) {
    img.draw_line(x0 + i, y0 - outside, x0 + i, y1 + outside, color);  // left
    img.draw_line(x0 - outside, y0 + i, x1 + outside, y0 + i, color);  // top
    img.draw_line(x1 + i, y0 - outside, x1 + i, y1 + outside, color);  // right
    img.draw_line(x0 - outside, y1 + i, x1 + outside, y1 + i, color);  // bot
  }
}

void cimg_draw_text(CImg<unsigned char> &img, int x, int y, const char *text,
                    const unsigned char *bg_color,
                    const unsigned char *text_color = NULL,
                    float opacity=0.5, int font_size = 20) {
  /* wrapper for cimg draw_text */
  if (!text_color) {
    const unsigned char white[] = {255, 255, 255};
    text_color = white;
  }
  img.draw_text(x, y, text, text_color, bg_color, opacity, font_size);
  img.draw_text(x, y, text, text_color, 0, 1, font_size);
}

void save_detection_results(std::string filename, std::vector<float> &dets,
                            std::vector<std::string> class_names, float thresh) {
  // id, score, xmin, ymin, xmax, ymax
  assert (dets.size() % 6 == 0);
  fs::FileEditor fe(filename, true);
  if (!fe.is_open()) {
    auto logger = log::get_logger("default");
    logger->warn("Unable to open result file to write: ") << filename;
    return;
  }

  for (std::size_t i = 0; i < dets.size(); i += 6) {
    if (dets[i] < 0) continue;  // not an object
    int id = static_cast<int>(dets[i]);
    float score = dets[i + 1];
    if (score < thresh) continue;
    float xmin = dets[i + 2];
    float ymin = dets[i + 3];
    float xmax = dets[i + 4];
    float ymax = dets[i + 5];
    if (class_names.size() > 0 && id < class_names.size()) {
      fe << class_names[id];
    } else {
      fe << id;
    }
    fe << "\t" << score << "\t" << xmin << "\t" << ymin << "\t"
      << xmax << "\t" << ymax << os::endl();
  }
  fe.flush();
  fe.close();
}

CImg<unsigned char> zimage_to_cimg(Image &zimg) {
  CImg<unsigned char> cimg(zimg.cols(), zimg.rows(), 1, zimg.channels());
  unsigned char *ptr = zimg.ptr();
  for (int r = 0; r < cimg.height(); ++r) {
    for (int c = 0; c < cimg.width(); ++c) {
      for (int k = 0; k < cimg.spectrum(); ++k) {
        cimg(c, r, 0, k) = *(ptr++);
      }
    }
  }
  return cimg;
}

Image cimg_to_zimage(CImg<unsigned char> &cimg) {
  assert (cimg.depth() == 1);  // depth image not supported
  Image zimg(cimg.height(), cimg.width(), cimg.spectrum());
  // use high level access, this may impact performance a little bit, but safer
  for (int r = 0; r < cimg.height(); ++r) {
    for (int c = 0; c < cimg.width(); ++c) {
      for (int k = 0; k < cimg.spectrum(); ++k) {
        zimg(r, c, k) = cimg(c, r, 0, k);
      }
    }
  }
  return zimg;
}
void cimg_visualize_detections(CImg<unsigned char> &img, std::vector<float> &dets,
                               std::vector<std::string> &class_names,
                               float visu_thresh) {
  int width = img.width();
  int height = img.height();

  // generate random colors
  int num_classes = static_cast<int>(class_names.size());
  if (num_classes < 1) num_classes = 1;
  std::vector<std::vector<unsigned char>> colors;
  std::srand(std::time(0));
  for (int c = 0; c < num_classes; ++c) {
    std::vector<unsigned char> color;
    color.push_back(std::rand() % 255);
    color.push_back(std::rand() % 255);
    color.push_back(std::rand() % 255);
    colors.push_back(color);
  }

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
    const unsigned char *color = colors[0].data();
    std::ostringstream ss;
    ss.precision(4);
    ss << score;
    std::string text = ss.str();
    if (id < num_classes) {
      color = colors[id].data();
      text = class_names[id] + ": " + text;
    }
    int font_size = 20;
    cimg_draw_box(img, xmin, ymin, xmax, ymax, color, 3);
    cimg_draw_text(img, xmin, ymin - font_size, text.c_str(), color, NULL, 0.5, font_size);
  }
}

void display_cv(std::string name, std::vector<float> &dets, std::vector<std::string> &class_names,
              float visu_thresh) {

  Mat img = imread(name);
  int width = img.cols;
  int height = img.rows;

  // generate random colors
  int num_classes = static_cast<int>(class_names.size());
  if (num_classes < 1) num_classes = 1;
  std::vector<std::vector<unsigned char>> colors;
  std::srand(std::time(0));
  for (int c = 0; c < num_classes; ++c) {
    std::vector<unsigned char> color;
    color.push_back(std::rand() % 255);
    color.push_back(std::rand() % 255);
    color.push_back(std::rand() % 255);
    colors.push_back(color);
  }

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

    rectangle(img, cv::Point2f(xmin,ymin), cv::Point2f(xmax, ymax), Scalar(0,255,0));
  }
  imshow("reslut", img);
  waitKey();
                              
}

void visualize_detection(std::string img_path,
               std::vector<float> &detections,
               float visu_thresh,
               int max_disp_size,
               std::vector<std::string> class_names,
               std::string out_file) {
  display_cv(img_path, detections , class_names, visu_thresh);
  // Image image(img_path.c_str());
  // Image bak_img = image;

  // // Mat image_mat = imread(img_path.c_str());
  // // Mat bak_img_mat = image_mat.clone();
  
  // // resize for display  // // save results
  // // if (!result_file.empty()) {
  // //   det::save_detection_results(result_file, dets, class_names);
  // // }
  // if (max_disp_size > 0) {
  //   float max_size = max_disp_size;
  //   float ratio1 = max_size / image.rows();
  //   float ratio2 = max_size / image.cols();
  //   float ratio = ratio1 > ratio2 ? ratio2 : ratio1;
  //   bak_img.resize(ratio);
  // }
  
  // CImg<unsigned char> canvas = zimage_to_cimg(bak_img);
  // cimg_visualize_detections(canvas, detections, class_names, visu_thresh);

  // // save drawings if required
  // if (!out_file.empty()) {
  //   Image oimg = cimg_to_zimage(canvas);
  //   try {
  //     oimg.save(out_file.c_str(), 100);
  //   } catch (std::exception &e) {
  //     auto logger = log::get_logger("default");
  //     logger->error() << e.what();
  //   }
  // }

  // // display
  // CImgDisplay main_disp(canvas, "detection");
  // while (!main_disp.is_closed()) {
  //   main_disp.wait();
  // }
}



void GetMeanFile(const std::string image_file, mx_float* image_data,
                const int channels, const cv::Size resize_size)
                {
    // Read all kinds of file into a BGR color 3 channels image
    cv::Mat im_ori = cv::imread(image_file, 1);

    if (im_ori.empty()) {
        std::cerr << "Can't open the image. Please check " << image_file << ". \n";
        assert(false);
    }

    cv::Mat im;

    cv::resize(im_ori, im, resize_size);

    // Better to be read from a mean.nb file
    float mean = 117.0;

    int size = im.rows * im.cols * channels;

    mx_float* ptr_image_r = image_data;
    mx_float* ptr_image_g = image_data + size / 3;
    mx_float* ptr_image_b = image_data + size / 3 * 2;

    for (int i = 0; i < im.rows; i++) {
        uchar* data = im.ptr<uchar>(i);

        for (int j = 0; j < im.cols; j++) {
            if (channels > 1)
            {
                mx_float b = static_cast<mx_float>(*data++) - mean;
                mx_float g = static_cast<mx_float>(*data++) - mean;
                *ptr_image_g++ = g;
                *ptr_image_b++ = b;
            }

            mx_float r = static_cast<mx_float>(*data++) - mean;
            *ptr_image_r++ = r;

        }
    }
}

Detector::Detector(std::string model_prefix, int epoch, int width,
                   int height, float mean_r, float mean_g, float mean_b,
                   int device_type, int device_id) {
  auto logger = log::get_logger("default");
  if (epoch < 0 || epoch > 9999) {
    logger->error("Invalid epoch number: ") << epoch;
    exit(-1);
  }
  std::string model_file = model_prefix + "-" + fmt::int_to_zero_pad_str(epoch, 4) + ".params";
  if (!os::is_file(model_file)) {
    std::cerr << "Model file: " << model_file << " does not exist" << std::endl;
    exit(-1);
  }
  std::string json_file = model_prefix + "-symbol.json";
  if (!os::is_file(json_file)) {
    std::cerr << "JSON file: " << model_file << " does not exist" << std::endl;
    exit(-1);
  }
  if (width < 1 || height < 1) {
    std::cerr << "Invalid width or height: " << width << "," << height << std::endl;
    exit(-1);
  }
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

  // load model
  std::ifstream param_file(model_file, std::ios::binary | std::ios::ate);
  if (!param_file.is_open() ) {
    std::cerr << "Unable to open model file: " << model_file << std::endl;
    exit(-1);
  }
  std::streamsize size = param_file.tellg();
  param_file.seekg(0, std::ios::beg);
  buffer_.resize(size);

  std::ifstream json_handle(json_file, std::ios::ate);
  std::string json_buffer;
  json_buffer.reserve(json_handle.tellg());
  json_handle.seekg(0, std::ios::beg);
  json_buffer.assign((std::istreambuf_iterator<char>(json_handle)), std::istreambuf_iterator<char>());
  if (json_buffer.size() < 1) {
    std::cerr << "invalid json file: " << json_file << std::endl;
    exit(-1);
  }

  if (param_file.read(buffer_.data(), size)) {
    MXPredCreate(json_buffer.c_str(), buffer_.data(), static_cast<int>(size),
      device_type, device_id, 1, input_keys, input_shape_indptr, input_shape_data,
      &predictor_);
  } else {
    std::cerr << "Unable to read model file: " << model_file << std::endl;
    exit(-1);
  }
}

std::vector<float> Detector::detect(std::string in_img) {
  auto logger = log::get_logger("default");
  if (!os::is_file(in_img)) {
    std::cerr << "Image file: " << in_img << " does not exist" << std::endl;
    exit(-1);
  }
  Image image(in_img.c_str());
  Mat img_mat = imread(in_img);
  imshow("test", img_mat);
  waitKey(1000);


  if (image.empty()) {
    std::cerr << "Unable to load image file: " << in_img << std::endl;
    exit(-1);
  }
  if (image.channels() != 3) {
    std::cerr << "RGB image required" << std::endl;
    exit(-1);
  }

      // Just a big enough memory 1000x1000x3
    int image_size = width_ * height_ * 3;
    std::vector<mx_float> image_data = std::vector<mx_float>(image_size);

    //-- Read Mean Data
    GetMeanFile(in_img, image_data.data(), 3, cv::Size(width_, height_));

    //-- Set Input Image
    MXPredSetInput(predictor_, "data", image_data.data(), image_size);

  // use model to forward
  mx_uint *shape = NULL;
  mx_uint shape_len = 0;
  // MXPredSetInput(predictor_, "data", in_data.data(), static_cast<mx_uint>(size));
  time::Timer timer;
  MXPredForward(predictor_);
  MXPredGetOutputShape(predictor_, 0, &shape, &shape_len);
  mx_uint tt_size = 1;
  for (mx_uint i = 0; i < shape_len; ++i) {
    tt_size *= shape[i];
  }
  assert(tt_size % 6 == 0);
  std::vector<float> outputs(tt_size);
  MXPredGetOutput(predictor_, 0, outputs.data(), tt_size);
  logger->info("Forward elapsed time: ") << timer.to_string();
  return outputs;
}
}  // namespace det
