#-------------------------------------------------
#
# Project created by QtCreator 2018-06-07T17:34:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test2
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
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui


win32: LIBS += -LE:/opencv-3.4.1/build/x64/vc15/lib/ -lopencv_world341

INCLUDEPATH += E:/opencv-3.4.1/build/include
DEPENDPATH += E:/opencv-3.4.1/build/include

win32:!win32-g++: PRE_TARGETDEPS += E:/opencv-3.4.1/build/x64/vc15/lib/opencv_world341.lib
else:win32-g++: PRE_TARGETDEPS += E:/opencv-3.4.1/build/x64/vc15/lib/libopencv_world341.a

win32: LIBS += -LE:/dlib-19.10/Project/lib/ -ldlib

INCLUDEPATH += E:/dlib-19.10/Project/include
DEPENDPATH += E:/dlib-19.10/Project/include

win32:!win32-g++: PRE_TARGETDEPS += E:/dlib-19.10/Project/lib/dlib.lib
else:win32-g++: PRE_TARGETDEPS += E:/dlib-19.10/Project/lib/libdlib.a
