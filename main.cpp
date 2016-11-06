//main.cpp

#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

int main(int argc, char** argv)
{
	//---Timer---//
	std::clock_t start = std::clock();
	double timer_interval = 0.5;

	//---Haar cascades---//
	cv::CascadeClassifier face_cascade, eye_cascade;

	if (!face_cascade.load("./haarcascades/haarcascade_frontalface_alt2.xml"))
	{
		std::cerr << "ERROR: cannot cascade file for face";
		return 1;
	}

	if (!eye_cascade.load("./haarcascades/haarcascade_eye.xml"))
	{
		std::cerr << "ERROR: cannot load cascade file for eyes";
		return 1;
	}

	//---Webcam---//
	cv::VideoCapture capture(0); //-1, 0, 1 device id
	if (!capture.isOpened())
	{
		std::cerr << "ERROR: Cannot initialise camera";
		return 1;
	}

	cv::Mat cap_img, gray_img;
	std::vector<cv::Rect> faces, eyes;

	while (true)
	{
		if (cv::waitKey(1) == 27) //Press esc to exit
			break;

		if ((std::clock() - start) / (double) CLOCKS_PER_SEC > timer_interval)
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

			start = std::clock();
		}
	}
	
	return 0;
}