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

class overviewWindow
{
public:
	static overviewWindow& getInstance() {
		static overviewWindow instance;
		return instance;
	}
	void initialize(int width = 500, int height = 300);
	void addText(const string msg, int x, int y, int size = 1, Scalar color = Scalar(0, 0, 0));
	void addLine(int x1, int y1, int x2, int y2, int thickness = 1, Scalar color = Scalar(0, 0, 0));
	void addCircle(int x, int y, int r, int thickness, Scalar color = Scalar(0, 0, 0), bool solid = false);
	void addRectangle(int x1, int y1, int x2, int y2, int thickness = 1, Scalar color = Scalar(0, 0, 0), bool solid = false);
private:
	Mat canvas; 
	inline void update() {
		imshow("overviewWindow", canvas);
	}

};

