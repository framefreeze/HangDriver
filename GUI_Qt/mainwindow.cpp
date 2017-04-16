#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->open_but, SIGNAL(clicked()), this, SLOT(open_cam()));
    connect(&cam_timer, &QTimer::timeout, this, &MainWindow::updata_img);
    fgps = imread("/Users/DavidWang/Documents/test/opencv/qt/gui_qt/data/fgps.png");
    if(fgps.data){
        cvtColor(fgps, fgps, COLOR_BGRA2RGB);
        fgps_q = QImage((uchar*)(fgps.data), fgps.cols, fgps.rows, QImage::Format_RGB888);
        ui->fgps_show->setPixmap(QPixmap::fromImage(fgps_q).scaled(ui->fgps_show->size()));
        this->update();
    }
}

MainWindow::~MainWindow()
{
    camera.release();
    delete ui;
}

void MainWindow::open_cam(){
    camera.open(0); // open camera
    if(camera.isOpened()){
        cam_timer.start(10);
    }
}

void MainWindow::updata_img(){
    camera >> frame;
    if(frame.data){
        cvtColor(frame, frame, COLOR_BGR2RGB);
        display = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
        ui->QImage_dis->setPixmap(QPixmap::fromImage(display).scaled(ui->QImage_dis->size()));
        this->update();
    }
}
