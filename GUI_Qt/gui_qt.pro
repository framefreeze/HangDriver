#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T17:16:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = gui_qt
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp detect_display.cpp back_detection.cpp
#    debug/moc_mainwindow.cpp

HEADERS  += mainwindow.h detect_display.h back_detection.h
#    debug/ui_mainwindow.h

FORMS    += mainwindow.ui

## MAC
#MXNETDIR = /Users/DavidWang/Documents/learn/mxnet-ssd/mxnet
#OPENCVDIR = /usr/local/Cellar/opencv/3.2.0/include

## Bob ubuntu
MXNETDIR = /home/framefreeze/Documents/learn/mxnet-ssd/mxnet
OPENCVDIR = /home/framefreeze/diy_install/opencv/include/

INCLUDEPATH += $${MXNETDIR}/include $${OPENCVDIR} $${OPENCVDIR}/opencv /home/framefreeze/diy_install/cuda-8.0/include/



# MAC
# LIBS += -L/usr/local/Cellar/opencv/3.2.0/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
#LIBS += $${MXNETDIR}/lib/libmxnet.so

# Bob ubuntu
LIBS += -L/home/framefreeze/diy_install/opencv/lib -lopencv_cudabgsegm -lopencv_cudaobjdetect -lopencv_cudastereo -lopencv_stitching -lopencv_cudafeatures2d -lopencv_superres -lopencv_cudacodec -lopencv_videostab -lopencv_cudaoptflow -lopencv_cudalegacy -lopencv_cudawarping -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dpm -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_plot -lopencv_dnn -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_photo -lopencv_imgcodecs -lopencv_cudaimgproc -lopencv_cudafilters -lopencv_imgproc -lopencv_cudaarithm -lopencv_core -lopencv_cudev
LIBS += $${MXNETDIR}/lib/libmxnet.so
LIBS += -L/home/framefreeze/diy_install/cuda-8.0/lib64 -lcublas -lcurand -lcudnn -L/home/framefreeze/diy_install/cuda-8.0/lib64/stubs -lcuda

DISTFILES += \
    data/fgps.png

