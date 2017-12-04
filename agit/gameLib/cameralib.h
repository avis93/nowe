                     /**
* @file HoughCircle_Demo.cpp
* @brief Demo code for Hough Transform
* @author OpenCV team
*/
#define cameralib_h
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;


	// windows and trackbars name
	const string windowName = "Hough Circle Detection Demo";
	const string cannyThresholdTrackbarName = "Canny threshold";
	const string accumulatorThresholdTrackbarName = "Accumulator Threshold";
	const string usage = "Usage : tutorial_HoughCircle_Demo <path_to_input_image>\n";

	// initial and max values of the parameters of interests.
	const int cannyThresholdInitialValue = 100;
	const int accumulatorThresholdInitialValue = 50;
	const int maxAccumulatorThreshold = 200;
	const int maxCannyThreshold = 255;

	int HoughDetection(const Mat& src_gray, const Mat& src_display, int cannyThreshold, int accumulatorThreshold)
	{
		// will hold the results of the detection
		vector<Vec3f> circles;
		// runs the actual detection
		HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows / 8, cannyThreshold, accumulatorThreshold, 0, 0);

		// clone the colour, input image for displaying purposes
		Mat display = src_display.clone();

		int stepSize = 160;
		int stepSize1 = 213;
		int flag = 0;
		int k = 0;


		int width = display.size().width;
		int height = display.size().height;

		for (int i = 0; i < 9; i++)
		{
			cv::line(display, Point(0, flag), Point(width, flag), cv::Scalar(0, 255, 255));
			flag += stepSize;
		}
		flag = 0;
		for (int i = 0; i < 9; i ++)
		{
			cv::line(display, Point(flag, 0), Point(flag, height), cv::Scalar(0, 255, 255));
			flag += stepSize1;
		}


		for (size_t i = 0; i < circles.size(); i++)
		{


			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			if (circles[i][0] < 213 && circles[i][1] < 160)
			{
				k = 1;
				return k;
			}
			if (circles[i][0] < 426 && circles[i][0] > 213 && circles[i][1] < 160)
			{
				k = 2;
				return k;
			}
			if (circles[i][0] < 640 && circles[i][0] > 426 && circles[i][1] < 160)
			{
				k = 3;
				return k;
			}
			if (circles[i][0] < 213 && circles[i][1] < 320 && circles[i][1] > 160)
			{
				k = 4;
				return k;
			}
			if (circles[i][0] < 426 && circles[i][0] > 213 && circles[i][1] < 320 && circles[i][1] > 160)
			{
				k = 5;
				return k;
			}
			if (circles[i][0] < 640 && circles[i][0] > 426 && circles[i][1] < 320 && circles[i][1] > 160)
			{
				k = 6;
				return k;
			}
			if (circles[i][0] < 213 && circles[i][1] < 480 && circles[i][1] > 320)
			{
				k = 7;
				return k;
			}
			if (circles[i][0] < 426 && circles[i][0] > 213 && circles[i][1] < 480 && circles[i][1] > 320)
			{
				k = 8;
				return k;
			}
			if (circles[i][0] < 640 && circles[i][0] > 426 && circles[i][1] < 480 && circles[i][1] > 320)
			{
				k = 9;
				return k;
			}

			int radius = cvRound(circles[i][2]);
			// circle center
			circle(display, center, 10, Scalar(0, 255, 0), -1, 8, 0);
			// circle outline
			circle(display, center, radius, Scalar(0, 0, 255), 3, 8, 0);

		}

		// shows the results
		imshow(windowName, display);
	}


int CameraVision()
{
	Mat src, src_gray;
	VideoCapture stream1(1);

	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//declare and initialize both parameters that are subjects to change
	int cannyThreshold = cannyThresholdInitialValue;
	int accumulatorThreshold = accumulatorThresholdInitialValue;
	int res;


	while (true) {
		Mat cameraFrame;
		stream1 >> cameraFrame;
		// Convert it to gray
		cvtColor(cameraFrame, src_gray, COLOR_BGR2GRAY);
		// Reduce the noise so we avoid false circle detection
		GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);

		// create the main window, and attach the trackbars
		namedWindow(windowName, WINDOW_NORMAL);
		resizeWindow("Hough Circle Detection Demo", 640, 480);
		createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
		createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold, maxAccumulatorThreshold);


		cannyThreshold = std::max(cannyThreshold, 1);
		accumulatorThreshold = std::max(accumulatorThreshold, 1);

		//runs the detection, and update the display
		res=HoughDetection(src_gray, cameraFrame, cannyThreshold, accumulatorThreshold);


		if (waitKey(30) >= 0) break;
	}

	return res;
}
#endif
