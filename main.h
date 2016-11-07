#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
//Check http ://stackoverflow.com/questions/19321804/this-function-or-variable-may-be-unsafe-visual-studio about localtime() vs  localtime_s()

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
const std::string PROFILE_FACE_CASCADE_LOCATION = "./haarcascades/haarcascade_profileface.xml";

//debug shit
const bool PRINTCSV = false;
const bool DEBUG = true;

//window appearances
const int MAX_FACES_PER_ROW = 3; //how many normalized faces to show per row before wrapping down to next row