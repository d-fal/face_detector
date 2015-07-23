#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T13:50:29
#
#-------------------------------------------------

QT       += core gui sql network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zbar_rev01
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    qrscanner.cpp \
    mainthread.cpp \
    facedetector2.cpp \
    uploadimage.cpp \
    barcodethread.cpp \
    checkcameras.cpp \
    networkmanagement.cpp \
    showmessages.cpp

HEADERS  += mainwindow.h \
    qrscanner.h \
    mainthread.h \
    facedetector2.h \
    uploadimage.h \
    barcodethread.h \
    checkcameras.h \
    networkmanagement.h \
    showmessages.h

FORMS    += mainwindow.ui \
    checkcameras.ui \
    networkmanagement.ui

INCLUDEPATH += C:\OpenCV\myBuild\install\include

LIBS += -LC:\\OpenCV\\myBuild\\install\\x64\\mingw\\lib \
    libopencv_core249d \
    libopencv_highgui249d \
    libopencv_imgproc249d \
    libopencv_features2d249d \
    libopencv_calib3d249d \
    libopencv_flann249d \
    libopencv_gpu249d \
    libopencv_nonfree249d \
    libopencv_objdetect249d \
    libopencv_ocl249d \
    libopencv_photo249d \
    libopencv_stitching249d \
    libopencv_superres249d \
    libopencv_video249d \
    libopencv_videostab249d\
    libopencv_ml249d \
    libopencv_contrib249d.dll

OTHER_FILES += \
    myapp.rc
RC_FILE = myapp.rc\

RESOURCES += \
    repository.qrc
