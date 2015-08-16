#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T11:22:57
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uWho
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webget.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    webget.h \
    config.h

FORMS    += mainwindow.ui \
    webget.ui \
    config.ui


INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_shape
LIBS += -lopencv_stitching
LIBS += -lopencv_superres
LIBS += -lopencv_videostab
LIBS += -lopencv_adas
LIBS += -lopencv_bgsegm
LIBS += -lopencv_bioinspired
LIBS += -lopencv_ccalib
LIBS += -lopencv_cvv
LIBS += -lopencv_datasets
LIBS += -lopencv_face
LIBS += -lopencv_latentsvm
LIBS += -lopencv_objdetect
LIBS += -lopencv_line_descriptor
LIBS += -lopencv_optflow
LIBS += -lopencv_reg
LIBS += -lopencv_rgbd
LIBS += -lopencv_saliency
LIBS += -lopencv_stereo
LIBS += -lopencv_surface_matching
LIBS += -lopencv_text
LIBS += -lopencv_tracking
LIBS += -lopencv_video
LIBS += -lopencv_ximgproc
LIBS += -lopencv_calib3d
LIBS += -lopencv_features2d
LIBS += -lopencv_ml
LIBS += -lopencv_flann
LIBS += -lopencv_xobjdetect
LIBS += -lopencv_xphoto
LIBS += -lopencv_highgui
LIBS += -lopencv_videoio
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_photo
LIBS += -lopencv_imgproc
LIBS += -lopencv_core
LIBS += -lopencv_hal

RESOURCES += \
    resources.qrc
