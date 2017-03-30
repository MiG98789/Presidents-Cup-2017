#pragma once
#include <iostream>

//---OpenCV header files---//
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

const int MAX_Q_SIZE = 100; //Maximum queue size

class FixedQueue
{
	Point* arr;
	int index = 0; //Points to latest element to be rewritten
	int size;

public:
	FixedQueue(int size_rhs);
	~FixedQueue();
	void enqueue(Point n);
	Point get(int n);
	void incrementXval(int n);
	Point* getOrderedArr();
};

