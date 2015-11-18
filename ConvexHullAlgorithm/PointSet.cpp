#include "PointSet.h"
#include <assert.h>
#include "Point.h"

PointSet::PointSet() {
}


PointSet::~PointSet() {
}

PointSet::PointIter PointSet::getPointIter(const Point* position) {
	for(PointIter point = points.begin(); point != points.end(); ++point) {
		if(*point == position) {
			return point;
		}
	}
	return points.end();
}

void PointSet::addPoint(const Point& point) {
	points.push_back(&point);
}

void PointSet::removePoint(const Point& point) {
	PointIter pointIter = getPointIter(&point);
	if(pointIter != points.end()) {
		points.erase(pointIter);
	}
}

void PointSet::addPoints(std::vector<const Point*> points) {
	points.insert(std::end(this->points), std::begin(points), std::end(points));
}

void PointSet::clear() {
	points.clear();
}

int PointSet::getSize() const {
	return points.size();
}

const Point* PointSet::getPoint(int index) const {
	assert(index >= 0 && index < points.size());
	return points[index];
}
