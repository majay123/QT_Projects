#-------------------------------------------------
#
# Project created by QtCreator 2019-11-17T21:03:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XVideo
TEMPLATE = app


SOURCES += xvideo.cpp\
        main.cpp \
    videothread.cpp \
    videowidget.cpp \
    imagepro.cpp \
    filter.cpp

HEADERS  += \
    videothread.h \
    videowidget.h \
    xvideo.h \
    imagepro.h \
    filter.h

FORMS    += xvideo.ui

INCLUDEPATH += D:\QT5.5\build\install\include
            D:\QT5.5\build\install\include\opencv2

LIBS +=  D:\QT5.5\build\lib\libopencv_*.a

INCLUDEPATH += D:\QT5.5\build\OpencvBuildLib\include
            D:\QT5.5\build\OpencvBuildLib\include\opencv2\

OBJECTS_DIR  = tmp
MOC_DIR      = tmp

QMAKE_CFLAGS += -std=c99
CONFIG   += c++11 (Qt5)

RESOURCES += \
    resource.qrc



