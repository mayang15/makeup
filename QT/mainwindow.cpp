#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;
using namespace cv;
QImage Mat2QImage(cv::Mat cvImg);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    flag = 1;
    ui->lineEdit->setText("204");
    ui->lineEdit_2->setText("51");
    ui->lineEdit_3->setText("51");
    ui->lineEdit_4->setText("0.8");
    R = 204;
    G = 51;
    B = 51;
    opacity = 0.8;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setRGB1()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(setRGB2()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(setRGB3()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(setRGB4()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(setRGB5()));
    connect(ui->Begin,SIGNAL(clicked()), this, SLOT(makeup()));
    connect(ui->closeCam,SIGNAL(clicked()), this, SLOT(closeCam()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setRGB1()
{
    R = 204;
    G = 51;
    B = 51;
}
void MainWindow::setRGB2()
{
    R = 139;
    G = 26;
    B = 26;
}
void MainWindow::setRGB3()
{
    R = 147;
    G = 0;
    B = 110;
}
void MainWindow::setRGB4()
{
    R = 255;
    G = 92;
    B = 108;
}
void MainWindow::setRGB5()
{
    R = ui->lineEdit->text().toInt();
    G = ui->lineEdit_2->text().toInt();
    B = ui->lineEdit_3->text().toInt();
}
void MainWindow::closeCam()
{
    flag = 0;
    cv::destroyAllWindows();
    this->close();
    waitKey(0);
}
void MainWindow::makeup()
{
    opacity = ui->lineEdit_4->text().toDouble();
    cout<<"RGBO:"<<R<<' '<<G<<' '<<B<<' '<<opacity<<' '<<endl;
    try
        {
            cv::VideoCapture cap(0);
            if (!cap.isOpened())
            {
                cerr << "Unable to connect to camera" << endl;
            }

            //image_window win;

            // Load face detection and pose estimation models.
            frontal_face_detector detector = get_frontal_face_detector();
            shape_predictor pose_model;
            deserialize("C:/Users/mayang/Documents/test2/shape_predictor_68_face_landmarks.dat") >> pose_model;

            // Grab and process frames until the main window is closed by the user.
            while (flag)
            {
                // Grab a frame
                cv::Mat temp;
                cap >> temp;

                cv_image<bgr_pixel> cimg(temp);       // only stores pointer, no deep copy
                // Detect faces
                std::vector<dlib::rectangle> faces = detector(cimg);
                // Find the pose of each face.
                std::vector<full_object_detection> shapes;
                for (unsigned long i = 0; i < faces.size(); ++i)
                    shapes.push_back(pose_model(cimg, faces[i]));

                if (!shapes.empty()) {
                    for (int i = 0; i < 68; i++) {
                        //circle(temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
                        //  shapes[0].part(i).x();
                    }
                }

                if (!shapes.empty()) {
                    // choose a color that you like

                    int lip_color[3];  // BGR
                    lip_color[0] = B;
                    lip_color[1] = G;
                    lip_color[2] = R;
                    //cout<<"RGBO:"<<R<<' '<<G<<' '<<B<<' '<<opacity<<' '<<endl;
                    //double opacity = 0.8;              // choose opacity [0,1]

                    // fillpoly确定嘴唇区域:mask
                    cv::Point root_points[1][20];
                    for (int i = 48; i < 68; i++) {
                        root_points[0][i - 48] = cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y());
                    }
                    const cv::Point* ppt[1] = { root_points[0] };
                    int npt[] = { 20 };
                    cv::Mat mask(temp.size(), temp.type(), cv::Scalar(0));
                    fillPoly(mask, ppt, npt, 1, cv::Scalar(255, 255, 255));

                    // adjust lip color using softlight
                    cv::Mat temp_copy = temp.clone();
                    for (int x = 0; x < temp.cols; x++) {
                        for (int y = 0; y < temp.rows; y++) {
                            if (mask.at<cv::Vec3b>(y, x) == cv::Vec3b(255, 255, 255)) {
                                cv::Scalar color = temp.at<cv::Vec3b>(y, x);
                                double b = color(0) / 255.0;
                                double g = color(1) / 255.0;
                                double r = color(2) / 255.0;
                                double cb, cg, cr;
                                if (lip_color[0] <= 128) {
                                    cb = (2 * lip_color[0] / 255.0 - 1)*(b - b * b) + b;
                                }
                                else {
                                    cb = (2 * lip_color[0] / 255.0 - 1)*(sqrt(b) - b) + b;
                                }
                                if (lip_color[1] <= 128) {
                                    cg = (2 * lip_color[1] / 255.0 - 1)*(g - g * g) + g;
                                }
                                else {
                                    cg = (2 * lip_color[1] / 255.0 - 1)*(sqrt(g) - g) + g;
                                }
                                if (lip_color[2] <= 128) {
                                    cr = (2 * lip_color[2] / 255.0 - 1)*(r - r * r) + r;
                                }
                                else {
                                    cr = (2 * lip_color[2] / 255.0 - 1)*(sqrt(r) - r) + r;
                                }
                                cb = cb * 255; cg = cg * 255; cr = cr * 255;
                                temp_copy.at<cv::Vec3b>(y, x) = cv::Vec3b(cb*opacity + color(0)*(1 - opacity), cg*opacity + color(1)*(1 - opacity), cr*opacity + color(2)*(1 - opacity));
                            }
                        }
                    }

                    // Poisson Image Editing
                    /*cv::Mat result;
                    cv::Point p(shapes[0].part(62).x(), shapes[0].part(60).y());
                    seamlessClone(temp, temp_copy, mask, p, result, 2);*/

                    //Display it all on the screen
                    imshow("Make up -- Lip", temp_copy);
                    cv::waitKey(1);

                }
            }
        }
        catch (serialization_error& e)
        {
            cout << "You need dlib's default face landmarking model file to run this example." << endl;
            cout << "You can get it from the following URL: " << endl;
            cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            cout << endl << e.what() << endl;
            system("pause");
        }
        catch (exception& e)
        {
            cout << e.what() << endl;
        }
        system("pause");
}
