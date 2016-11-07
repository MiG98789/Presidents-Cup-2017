//main.cpp

/*
TODO:
Input verification of course code
Make functions and cpp/h files
Check http://stackoverf	low.com/questions/19321804/this-function-or-variable-may-be-unsafe-visual-studio about  localtime() vs  localtime_s()
Different way to load haar cascades(?)
Find a way to use haarcascade_eye_tree_eyeglasses.xml since reflection from glasses prevent facial detection
DEBUG mode (no csv)
*/

#include "main.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	//---Timer---//
	clock_t  timerPrev = clock();	//Time of the last frame shot
	double timerDuration = 0;	//Time difference between current time and time of last frame shot

	//---Haar cascades---//
	CascadeClassifier faceCascade;	//Haar cascade classifier for face
	CascadeClassifier eyeCascade;	//Haar cascade classifier for eyes

	if (!faceCascade.load(FACE_CASCADE_LOCATION)) {
		cerr << "ERROR: cannot load " << FACE_CASCADE_LOCATION;
		return 100;
	}
	if (!eyeCascade.load(EYE_CASCADE_LOCATION)) {
		cerr << "ERROR: cannot load " << EYE_CASCADE_LOCATION;
		return 101;
	}

	//---Webcam---//
	VideoCapture capture(0); //0 for default camera
	if (!capture.isOpened()) {
		cerr << "ERROR: Cannot initialise camera";
		return 200;
	}
	capture.set(CV_CAP_PROP_BUFFERSIZE, 1); //Set how many frames the buffer will store

	//---Course---//	

	string courseCode;	//Name of course
	if (PRINTCSV) {
		cout << "What is the course code? ";
		cin >> courseCode;
		transform(courseCode.begin(), courseCode.end(), courseCode.begin(), ::tolower);	//Make course code lowercase
	}

	//---Time information---//
	time_t start = time(0);	//Get time of start of lecture
	time_t now = time(0);	//Get current time
	struct tm* tmStart = localtime(&start);	//Convert current time into data structure
	struct tm* tmNow = localtime(&now);	//Create time structure for output of results
	string tString = "";	//Date format (24 characters): yyyy-mm-dd-hh-mm-ss
	ostringstream tTemp;	//Temporary string to store time information
	int timeElapsed = 0;	//Time since beginning of lecture in seconds
	int timeStart = (tmStart->tm_hour) * 60 + (tmStart->tm_min) * 60 + (tmStart->tm_sec);	//Time of start of lecture in seconds
	int timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);	//Time now in seconds

	tTemp << tmStart->tm_year + 1900;	//Get year since 1990
	tString += tTemp.str() + "-";
	tTemp.str(string());	//Clear stringstream
	tTemp << tmStart->tm_mon + 1;	//Get number of months since January
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_mday;	//Get day of the month
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_hour;	//Get hour since midnight
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_min;	//Get minute
	tString += tTemp.str() + "-";
	tTemp.str(string());
	tTemp << tmStart->tm_sec;	//Get seconds
	tString += tTemp.str();
	tTemp.str(string());

	//---Output file---//
	ofstream ofs;
	if (PRINTCSV) {
		string file = tString + '-' + courseCode + ".csv";
		ofs.open(file);	//Output file stream, data format: seconds since start of lecture, results\n
		ofs << tString << '-' << courseCode << '\n';
	}

	//---Frame information---//
	Mat frame;	//Captured frame
	Mat frameGray;	//To preprocess the captured frame
	vector< Rect> faces;	//Contain output of face detector
	vector< Rect> eyes;	//Contain output of eye detector

	while (true) {
		if (waitKey(1) == ESC) //Press esc to exit
			break;

		timerDuration = (clock() - timerPrev) / (double)CLOCKS_PER_SEC;

		if (timerDuration > TIMER_INTERVAL)
		{
			capture >> frame;	//Capture frame
			cvtColor(frame, frameGray, CV_BGR2GRAY);	//Convert frame from BGR to gray for easier processing
			equalizeHist(frameGray, frameGray);	//Normalise brightness and increase contrast: src,dest
			faceCascade.detectMultiScale(
				frameGray,
				faces,
				1.1,
				10,
				0 | CV_HAAR_SCALE_IMAGE,
				cvSize(0, 0),
				cvSize(300, 300)
			); //Detect faces, can also try CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING

			//---Loop through each face---//
			Mat frameNormalizedFaces;

			for (int i = 0; i < faces.size(); i++) {
				//---Detect size of each face---//
				Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);	//Find first point of the face
				Point pt2(faces[i].x, faces[i].y);	//Find point opposite to pt1
				Mat faceROI = frameGray(faces[i]);	//Stores current face of the loop

			   /*
			   //---Detect eyes---//
			   eyeCascade.detectMultiScale(
				   faceROI,
				   eyes,
				   1.1,
				   2,
				   0 | CV_HAAR_SCALE_IMAGE,
					Size(30, 30)
			   );
			   for (size_t j = 0; j < eyes.size(); j++)
			   {
					Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);	//Find the center of each eye
				   int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);	//Find the radius of each eye
				   circle(frame, center, radius,  Scalar(255, 0, 0), 2, 8, 0);	//Draw a circle around each eye
			   }*/
				resize(faceROI, faceROI, Size(100, 100));
				hconcat(faceROI, faceROI, frameNormalizedFaces);
				//hconcat(frameNormalizedFaces, faceROI, frameNormalizedFaces);
				//---Draw rectangles around each face---//
			}
			if (!frameNormalizedFaces.empty()) {
				imshow("NormalizedFaces", frameNormalizedFaces);
			}
			imshow("Result", frame);	//Output the processed image

			timeElapsed = timeNow - timeStart;

			cout << timeElapsed << "\tNumber of faces: " << faces.size() << endl;

			if (PRINTCSV) {
				ofs << timeElapsed << ',' << faces.size() << '\n';
			}

			now = time(0);
			tmNow = localtime(&now);
			timeNow = (tmNow->tm_hour) * 60 + (tmNow->tm_min) * 60 + (tmNow->tm_sec);
			timerPrev = clock(); //Reset comparison time for timer
		}
	}

	if (PRINTCSV) {
		ofs.close();	//Close output file stream
	}
	//No need to deintialise webcam

	return 0;
}