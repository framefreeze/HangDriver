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

private:
    Ui::MainWindow *ui;
    QTimer cam_timer;
    VideoCapture camera;
    Mat frame, fgps;
    QImage display, fgps_q;

    QImage mat2QImage(Mat img);

};

#endif // MAINWINDOW_H
