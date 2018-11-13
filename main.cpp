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

int main()
{
	try
	{
		cv::VideoCapture cap(0);
		if (!cap.isOpened())
		{
			cerr << "Unable to connect to camera" << endl;
			return 1;
		}

		//image_window win;  

		// Load face detection and pose estimation models.  
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

		// Grab and process frames until the main window is closed by the user.  
		while (1)
		{
			// Grab a frame  
			cv::Mat temp;
			cap >> temp;

			cv_image<bgr_pixel> cimg(temp);       // only stores pointer, no deep copy 
			// Detect faces   
			std::vector<rectangle> faces = detector(cimg);
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
				int lip_color[3] = { 51,51,204 };  // BGR
				double opacity = 0.8;              // choose opacity [0,1]

				// fillpoly locate lips: mask
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
				cv::Mat result;
				cv::Point p(shapes[0].part(62).x(), shapes[0].part(60).y());
				seamlessClone(temp, temp_copy, mask, p, result, 2);
				
				//Display it all on the screen  
				imshow("Dlib Face Demo", result);
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