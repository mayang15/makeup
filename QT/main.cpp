#include "mainwindow.h"
#include <QApplication>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");
    MainWindow w;
    w.setWindowTitle("Make up -- Lip");
    w.show();

    return a.exec();
}
