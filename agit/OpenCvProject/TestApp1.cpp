#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
   VideoCapture cap(0);

   Mat save_img;

   cap >> save_img;

   char Esc = 0;

   while (Esc != 27 && cap.isOpened()) {        
    bool Frame = cap.read(save_img);        
    if (!Frame || save_img.empty()) {       
        cout << "error: frame not read from webcam\n";      
        break;                                              
    }
    namedWindow("save_img", CV_WINDOW_NORMAL);  
    imshow("imgOriginal", save_img);            
    Esc = waitKey(1);
}
imwrite("test.jpg",save_img); 
}











/*
int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}*/