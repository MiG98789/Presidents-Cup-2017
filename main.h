//Main.h

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


//---Custom header files---//
#include "overviewWindow.h"

//---Global constant variables---//
//Buttons
const int ESC = 27;	//Escape button

//Timer
const double TIMER_INTERVAL = 0;	//Time between every shot (in seconds)

//Haar Cascades
const std::string FACE_CASCADE_LOCATION = "./haarcascades/haarcascade_frontalface_alt.xml";	//Location of Haar cascade classifier for face
const std::string EYE_CASCADE_LOCATION = "./haarcascades/haarcascade_eye.xml";	//Location of Haar cascade classifier for eyes
const std::string PROFILE_FACE_CASCADE_LOCATION = "./haarcascades/haarcascade_profileface.xml";	//Location of Haar cascade classifier for profile face

//Debug
const bool PRINTCSV = false;	//Output to a csv file if true
const bool DEBUG = true;		//Print all cout and cerr statements if true

//window appearances
const int FACE_SIZE = 100;	//Length of each square tile of a cropped-and-scaled face
const int MAX_FACES_PER_ROW = 3; //how many normalized faces to show per row before wrapping down to next row
