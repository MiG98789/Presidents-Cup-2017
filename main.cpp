//main.cpp

/*
TODO: 
Input verification of course code
Make functions and cpp/h files
check http://stackoverflow.com/questions/19321804/this-function-or-variable-may-be-unsafe-visual-studio about std::localtime() vs std::localtime_s()
*/

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//---Standard C++ header files---//
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <algorithm>
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
	std::clock_t  timerPrev = std::clock();	//Time of the last frame shot
	const double timerInterval = 0;	//Time between every shot (in seconds)
	double timerDuration = 0;	//Time difference between current time and time of last frame shot

	//---Haar cascades---//
	std::string faceCascadeLocation = "./haarcascades/haarcascade_frontalface_alt.xml";	//Location of Haar cascade classifier for face
	std::string eyeCascadeLocation = "./haarcascades/haarcascade_eye.xml";	//Location of Haar cascade classifier for eyes
	cv::CascadeClassifier faceCascade;	//Haar cascade classifier for face
	cv::CascadeClassifier eyeCascade;	//Haar cascade classifier for eyes

	if (!faceCascade.load(faceCascadeLocation))
	{
		std::cerr << "ERROR: cannot cascade file for face";
		return 100;
	}

	if (!eyeCascade.load(eyeCascadeLocation))
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

	//---Time information---//
	time_t start = time(0);	//Get time of start of lecture
	time_t now = time(0);	//Get current time
	struct tm* tmStart = std::localtime(&start);	//Convert current time into data structure
	struct tm* tmNow = std::localtime(&now);	//Create time structure for output of results
	std::string tString = "";	//Date format (24 characters): yyyy-mm-dd-hh-mm-ss
	std::ostringstream tTemp;	//Temporary string to store time information
	int timeElapsed = 0;	//Time since beginning of lecture in seconds
	int timeStart = (tmStart->tm_hour) * 60 + (tmStart->tm_min) * 60 + (tmStart->tm_sec);	//Time of start of lecture in seconds
	int timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);	//Time now in seconds

	tTemp << tmStart->tm_year + 1900;	//Get year since 1990
	tString += tTemp.str() + "-";
	tTemp.str(std::string());	//Clear stringstream
	tTemp << tmStart->tm_mon + 1;	//Get number of months since January
	tString += tTemp.str() + "-";
	tTemp.str(std::string());
	tTemp << tmStart->tm_mday;	//Get day of the month
	tString += tTemp.str() + "-";
	tTemp.str(std::string());
	tTemp << tmStart->tm_hour;	//Get hour since midnight
	tString += tTemp.str() + "-";
	tTemp.str(std::string());
	tTemp << tmStart->tm_min;	//Get minute
	tString += tTemp.str() + "-";
	tTemp.str(std::string());
	tTemp << tmStart->tm_sec;	//Get seconds
	tString += tTemp.str();
	tTemp.str(std::string());

	//---Course---//
	std::string courseCode;	//Name of course
	std::cout << "What is the course code? ";
	std::cin >> courseCode;
	std::transform(courseCode.begin(), courseCode.end(), courseCode.begin(), ::tolower);	//Make course code lowercase

	//---Output file---//
	std::string file = tString + "-" + courseCode + ".cvs";
	std::ofstream ofs(file);	//Output file stream, data format: seconds since start of lecture, results\n

	//---Frame information---//
	cv::Mat frame;	//Captured frame
	cv::Mat frameGray;	//To preprocess the captured frame
	std::vector<cv::Rect> faces;	//Contain output of face detector
	std::vector<cv::Rect> eyes;	//Contain output of eye detector

	while (true)
	{
		if (cv::waitKey(1) == 27) //Press esc to exit
			break;

		timerDuration = (std::clock() - timerPrev) / (double)CLOCKS_PER_SEC;

		if (timerDuration > timerInterval)
		{
			capture >> frame;	//Capture frame
			cvtColor(frame, frameGray, CV_BGR2GRAY);	//Convert frame from BGR to gray for easier processing
			equalizeHist(frameGray, frameGray);	//Normalise brightness and increase contrast
			faceCascade.detectMultiScale(frameGray, faces, 1.1, 10, 
				0 | CV_HAAR_SCALE_IMAGE, cvSize(0, 0), cvSize(300, 300)); //Detect faces, can also try CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING

			//---Loop through each face---//
			for (int i = 0; i < faces.size(); i++)
			{
				//---Detect size of each face---//
				cv::Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
				cv::Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1
				cv::Mat faceROI = frameGray(faces[i]);	//Stores current face of the loop

				//---Detect eyes---//
				eyeCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
				for (size_t j = 0; j< eyes.size(); j++)
				{
					cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);	//Find the center of each eye
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);	//Find the radius of each eye
					circle(frame, center, radius, cv::Scalar(255, 0, 0), 2, 8, 0);	//Draw a circle around each eye
				}
				
				//---Draw rectangles around each face---//
				rectangle(frame, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
			}
			imshow("Result", frame);	//Output the processed image

			timeElapsed = timeNow - timeStart;

			std::cout << timeElapsed << "\tNumber of faces: " << faces.size() << std::endl;

			ofs << timeElapsed << "," << faces.size() << "\n";

			now = time(0);
			tmNow = std::localtime(&now);
			timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);
			timerPrev = std::clock(); //Reset comparison time for timer
		}
	}

	ofs.close();	//Close output file stream
	
	//No need to deintialise webcam

	return 0;
}