#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

using namespace cv;

// bob ubuntu
char filename[300] = "/home/framefreeze/Documents/HangDriver/GUI_Qt/data/fgps2.png";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->open_but, SIGNAL(clicked()), this, SLOT(open_cam()));
    connect(ui->safe_det, SIGNAL(clicked()), this, SLOT(change2safe_mode()));
    connect(&cam_timer, &QTimer::timeout, this, &MainWindow::updata_img);

    fgps = imread(filename);
    if(fgps.data){
        cvtColor(fgps, fgps, COLOR_BGR2RGB);

//        fgps_q = QImage((uchar*)(fgps.data), fgps.cols, fgps.rows, fgps.cols*fgps.channels(),QImage::Format_RGB888);
        fgps_q = mat2QImage(fgps);
        ui->fgps_show->setPixmap(QPixmap::fromImage(fgps_q).scaled(ui->fgps_show->size()));
        //this->update();
    }

    safe_mode = false;
}

MainWindow::~MainWindow()
{
    camera.release();
    delete ui;
}

void MainWindow::open_cam(){
    camera.open(1); // open camera
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if(camera.isOpened()){
        cam_timer.start(10);
    }
}

void MainWindow::updata_img(){
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

void MainWindow::change2safe_mode(){
    safe_mode = !safe_mode;
}
QImage MainWindow::mat2QImage(Mat img){
    return QImage((uchar*)(img.data), img.cols, img.rows,img.cols*img.channels(),QImage::Format_RGB888);
}
