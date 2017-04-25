#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T17:16:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui_qt
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH +=  /usr/local/Cellar/opencv/3.2.0/include/opencv
INCLUDEPATH += /usr/local/Cellar/opencv/3.2.0/include

LIBS += -L/usr/local/Cellar/opencv/3.2.0/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

DISTFILES += \
    data/fgps.png

