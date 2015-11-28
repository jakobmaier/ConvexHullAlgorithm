#include "ConvexHull.h"
#include "Point.h"
#include <assert.h>

ConvexHull::ConvexHull() {
}

void ConvexHull::addPoint(const Point& point) {
	points.push_back(&point);
}

void ConvexHull::removeLastPoint() {
	points.pop_back();
}

void ConvexHull::clear() {
	points.clear();
}

int ConvexHull::getSize() const {
	return points.size();
}

const Point* ConvexHull::getPoint(int index) const {
	assert(index >= 0 && index < static_cast<int>(points.size()));
	return points[index];
}

bool ConvexHull::containsPoint(const Point& point) const {
	for(CPointIter iPoint = points.begin(); iPoint != points.end(); ++iPoint) {
		if(*iPoint == &point) {
			return true;
		}
	}
	return false;
}

ConvexHull::~ConvexHull() {
}
