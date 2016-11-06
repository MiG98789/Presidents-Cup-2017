//main.cpp

/*
*TODO:
*More tuning required
*
*/

//---Standard C++ header files---//
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>

//---OpenCV header files---//
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

int main(int argc, char** argv)
{
	//---Timer---//
	std::clock_t start = std::clock();	//Time of the last frame shot
	const double timer_interval = 10;	//Time between every shot (in seconds)
	double time_elapsed = 0;	//Time difference between current time and time of last frame shot

	//---Haar cascades---//
	cv::CascadeClassifier face_cascade;	//Haar cascade classifier for face
	cv::CascadeClassifier eye_cascade;	//Haar cascade classifier for eyes

	if (!face_cascade.load("./haarcascades/haarcascade_frontalface_alt.xml"))
	{
		std::cerr << "ERROR: cannot cascade file for face";
		return 100;
	}

	if (!eye_cascade.load("./haarcascades/haarcascade_eye.xml"))
	{
		std::cerr << "ERROR: cannot load cascade file for eyes";
		return 101;
	}

	//---Webcam---//
	cv::VideoCapture capture(0); //0 for default camera
	if (!capture.isOpened())
	{
		std::cerr << "ERROR: Cannot initialise camera";
		return 200;
	}
	capture.set(CV_CAP_PROP_BUFFERSIZE, 1); //Set how many frames the buffer will store

	cv::Mat cap_img, gray_img;	//Image matrices
	std::vector<cv::Rect> faces, eyes;

	while (true)
	{
		if (cv::waitKey(1) == 27) //Press esc to exit
			break;

		time_elapsed = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		if (time_elapsed > timer_interval)
		{
			capture >> cap_img;
			cvtColor(cap_img, gray_img, CV_BGR2GRAY);
			equalizeHist(gray_img, gray_img);
			face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
			for (int i = 0; i < faces.size(); i++)
			{
				cv::Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
				cv::Point pt2(faces[i].x, faces[i].y);
				cv::Mat faceROI = gray_img(faces[i]);
				eye_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				for (size_t j = 0; j< eyes.size(); j++)
				{
					cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(cap_img, center, radius, cv::Scalar(255, 0, 0), 2, 8, 0);
				}
				rectangle(cap_img, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
			}
			imshow("Result", cap_img);

			start = std::clock(); //Reset comparison time for timer
		}
	}
	
	return 0;
}