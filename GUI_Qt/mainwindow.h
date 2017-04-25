#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>

#include <opencv2/opencv.hpp>
#include "detect_display.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open_cam();
    void updata_img();
    void change2safe_mode();
    void set_dst_pos();

private:
    bool safe_mode;
    Ui::MainWindow *ui;
    QTimer cam_timer;
    VideoCapture camera;
    Mat frame, fgps;
    QImage display, fgps_q;
    detect_display detector;
    std::vector<float> dets;
    int dst_pos;

    QImage mat2QImage(Mat img);

};

#endif // MAINWINDOW_H
