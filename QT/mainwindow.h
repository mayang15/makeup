#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cmath>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setRGB1();
    void setRGB2();
    void setRGB3();
    void setRGB4();
    void setRGB5();
    void makeup();
    void closeCam();

private:
    Ui::MainWindow  *ui;
    int flag;
    int R;
    int G;
    int B;
    double opacity;
};

#endif // MAINWINDOW_H
