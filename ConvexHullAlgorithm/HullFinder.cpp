#include "HullFinder.h"
#include "PointSet.h"
#include "Point.h"


const Point* HullFinder::getPivot() const {
	// Returns the bottommost and rightmost point
	int pointCount = pointSet->getSize();
	if(pointCount == 0) {
		return nullptr;
	}
	const Point* pivot = pointSet->getPoint(0);
	for(int i = 1; i < pointCount; ++i) {
		const Point* other = pointSet->getPoint(i);
		if(other->y > pivot->y) {
			pivot = other;
		}
		if(other->y == pivot->y && other->x > pivot->x) {
			pivot = other;
		}
	}
	return pivot;
}

void HullFinder::findConvexHull() {
	delete hull;
	hull = new ConvexHull();
}

HullFinder::HullFinder(const PointSet& input)
: pointSet(&input)
, hull(nullptr) {
}

const ConvexHull* HullFinder::getHull() {
	if(hull == nullptr) {
		findConvexHull();
	}
	return hull;
}

HullFinder::~HullFinder() {
	delete hull;
}
