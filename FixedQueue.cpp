#include "FixedQueue.h"

FixedQueue::FixedQueue(int size_rhs = MAX_Q_SIZE) {
	size = size_rhs;
	arr = new Point[size];
	for (int i = 0; i < size; i++) {
		arr[i] = Point(0, 0);
	}
}

FixedQueue::~FixedQueue() {
	delete[] arr;
}

void FixedQueue::enqueue(Point n) {
	arr[index++] = n;
	if (index >= size) {
		index = 0;
	}
}

Point FixedQueue::get(int n) {
	return arr[(index + n) % size];
}

void FixedQueue::incrementXval(int n) {
	for (int i = 0; i < size; i++) {
		arr[i].x += n;
	}
}

Point* FixedQueue::getOrderedArr() {
	Point* ans = new Point[size];
	int ansPos = 0;
	for (int i = index - 1; i >= 0; i--) {
		ans[ansPos++] = arr[i];
	}
	for (int i = size - 1; i >= index; i--) {
		ans[ansPos++] = arr[i];
	}
	return ans;
}