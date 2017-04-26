#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
//#define APPLE 0
#define Debug 1
using namespace cv;

Ptr<BackgroundSubtractor> mog_cuda = cuda::createBackgroundSubtractorMOG2();
Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();

#ifdef APPLE
char fgps_file[300] = "/Users/DavidWang/Documents/learn/whq/HangDriver/GUI_Qt/data/fgps.png";
char mark[200] = "/Users/DavidWang/Documents/learn/whq/HangDriver/GUI/pentagram.png";
#else
char fgps_file[300] = "/home/framefreeze/Documents/HangDriver/GUI_Qt/data/fgps2.png";
char mark[200] = "/home/framefreeze/Documents/HangDriver/GUI_Qt/data/pentagram.png";
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->open_but, SIGNAL(clicked()), this, SLOT(open_cam()));
    connect(ui->safe_det, SIGNAL(clicked()), this, SLOT(change2safe_mode()));
    connect(&cam_timer, &QTimer::timeout, this, &MainWindow::cam_updata_img);
    connect(&fgps_timer, &QTimer::timeout, this, &MainWindow::fgps_updata_img);
    connect(ui->dst_set, SIGNAL(clicked()), this, SLOT(set_dst_pos()));

    fgps = imread(fgps_file);
    if(fgps.data){
        cvtColor(fgps, fgps, COLOR_BGR2RGB);
        fgps_q = mat2QImage(fgps);
        ui->fgps_show->setPixmap(QPixmap::fromImage(fgps_q).scaled(ui->fgps_show->size()));
        //this->update();
    }

    safe_mode = false;
    dst_pos = 30;
}

MainWindow::~MainWindow()
{
    camera.release();
    delete ui;
}

// 开始导航程序
void MainWindow::open_cam(){
    camera.open(0); // open camera
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if(camera.isOpened()){
        cam_timer.start(13);
    }

}

void MainWindow::cam_updata_img() {
    camera >> frame;
    if(frame.data){
        cvtColor(frame, frame, COLOR_BGR2RGB);
//        display = QImage((uchar*)(frame.data), frame.cols, frame.rows, frame.cols*frame.channels(),QImage::Format_RGB888);
        if(safe_mode){
            dets = detector.detect(frame);

            if (dets.empty()) {
                std::cout << "No detections found." << std::endl;
                imshow("display", frame);
            }
            else {
                // visualize detections
                detector.show_result(frame, frame, dets, 0.75);
            }
        }
        display = mat2QImage(frame);
        ui->QImage_dis->setPixmap(QPixmap::fromImage(display).scaled(ui->QImage_dis->size()));
        this->update();
    }
}

void MainWindow::fgps_updata_img() {
    gps_cam >> gps_frame;
    gps_frame = gps_frame(Rect(70,0,frame.cols-70,frame.rows));
    rotate(gps_frame, gps_frame, ROTATE_90_COUNTERCLOCKWISE);
    Point2f center;
    float r;
    gps_detector.detection(gps_frame, center, r);

#ifdef Debug
    imshow("fgps", gps_frame);
    std::cout << gps_detector.get_pos() << std::endl;
//    waitKey();
#endif

}

void MainWindow::change2safe_mode() {
    safe_mode = !safe_mode;
}

void MainWindow::set_dst_pos() {
    ui->dst_pos->setText("test");
    Mat dst_mark = imread(mark);
    cvtColor(fgps, fgps, COLOR_BGR2RGB);

    gps_cam.open(1); //open fgps camera
    if(gps_cam.isOpened()){
        fgps_timer.start(13);
    }
    else{
        std::cout << "output error" << std::endl;
    }
}

QImage MainWindow::mat2QImage(Mat img) {
    return QImage((uchar*)(img.data), img.cols, img.rows,img.cols*img.channels(),QImage::Format_RGB888);
}
