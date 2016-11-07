#pragma once
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

//---Custom header files---//

//---Global constant variables---//
//Escape button
const int ESC = 27;

//Timer
const double TIMER_INTERVAL = 0;	//Time between every shot (in seconds)

//Haar Cascades
const std::string FACE_CASCADE_LOCATION = "./haarcascades/haarcascade_frontalface_alt.xml";	//Location of Haar cascade classifier for face
const std::string EYE_CASCADE_LOCATION = "./haarcascades/haarcascade_eye.xml";	//Location of Haar cascade classifier for eyes