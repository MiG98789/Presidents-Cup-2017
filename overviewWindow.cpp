#include "overviewWindow.h"

void overviewWindow::initialize(int w, int h) {
	width = w;
	height = h;
	canvas = Mat(h, w, CV_8UC3, Scalar(255, 255, 255)); //reversed RGB (aka BGR)
	update();
}

void overviewWindow::addText(const string msg, int x, int y, int size, Scalar color){
	putText(canvas, msg, Point(x, y), FONT_HERSHEY_DUPLEX, size, color);
	update();
}

void overviewWindow::addLine(int x1, int y1, int x2, int y2, int thickness, Scalar color) {
	line(canvas, Point(x1, y1), Point(x2, y2), color, thickness);
	update();
}

void overviewWindow::addCircle(int x, int y, int r, int thickness, Scalar color, bool solid) {
	circle(canvas, Point(x, y), r, color, solid ? CV_FILLED : thickness);
	update();
}

void overviewWindow::addRectangle(int x1, int y1, int x2, int y2, int thickness, Scalar color, bool solid) {
	rectangle(canvas, Point(x1, y1), Point(x2, y2), color, solid ? CV_FILLED : thickness);
	update();
}

void overviewWindow::clear() {
	rectangle(canvas, Point(0, 0), Point(width, height), Scalar(255, 255, 255), CV_FILLED);
	update();
}
