#include "ConvexHull.h"
#include "Point.h"
#include <assert.h>
#include <sstream>
#include <iomanip>

ConvexHull::ConvexHull() {
}

ConvexHull::ConvexHull(std::stack<const Point*> hull) {
	while(!hull.empty()) {
		addPoint(*hull.top());
		hull.pop();
	}
}

void ConvexHull::addPoint(const Point& point) {
	points.push_back(&point);
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

std::string ConvexHull::String() const {
	std::stringstream ss;

	ss << points.size() << " Points" << std::endl;
	int idx = 0;
	for(const Point* p : points) {
		ss << "  (" << std::setfill(' ') << std::setw(3) << idx << ") ";
		ss << std::setfill(' ') << std::setw(8) << std::right << p->x;
		ss << " x ";
		ss << std::setfill(' ') << std::setw(8) << std::left << p->y;
		ss << std::endl;
		++idx;
	}

	return ss.str();
}