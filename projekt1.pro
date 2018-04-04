#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T10:20:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projekt1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    asf.cpp \
    bone.cpp \
    glwidget.cpp

HEADERS += \
        mainwindow.h \
    asf.h \
    bone.h \
    glwidget.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += "C:\OpenCV-3.3.0\opencv\build\include"

LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_core330.dll"
LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_highgui330.dll"
LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_imgcodecs330.dll"
LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_imgproc330.dll"
LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_features2d330.dll"
LIBS += "C:\OpenCV-3.3.0\opencv\my-build-mingw\bin\libopencv_calib3d330.dll"
LIBS += -lOpengl32

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/build
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

#INCLUDEPATH += $$(OPENCV_SDK_DIR)/include

#LIBS += -L$$(OPENCV_SDK_DIR)/x86/mingw/lib \
#        -lopencv_core320        \
#        -lopencv_highgui320     \
#        -lopencv_imgcodecs320   \
#        -lopencv_imgproc320     \
#        -lopencv_features2d320  \
#        -lopencv_calib3d320

RESOURCES += \
    myresources.qrc
