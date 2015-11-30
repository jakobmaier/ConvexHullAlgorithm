#define _USE_MATH_DEFINES
#include <cmath>

#include "PointSet.h"

#include <assert.h>
#include "Point.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "utils.h"

const float EPS = 0.00001f;



PointSet::PointSet()
: referencePoint(nullptr) {
}



PointSet::~PointSet() {
	clear();
}



PointSet::PointIter PointSet::getPointIter(const Point* point) {
	for(PointIter pIt = points.begin(); pIt != points.end(); ++pIt) {
		if(*pIt == point) {
			return pIt;
		}
	}
	return points.end();
}

bool PointSet::updateReferencePoint(const Point& newPoint) {
	if(referencePoint == nullptr) {
		referencePoint = &newPoint;
		return true;
	}
	if((newPoint.y < referencePoint->y) ||
	   (newPoint.y == referencePoint->y && newPoint.x > referencePoint->x)) {
		referencePoint = &newPoint;
		return true;
	}
	return false;
}


bool PointSet::addPoint(const Point& point) {
	points.push_back(&point);
	return updateReferencePoint(point);
}

bool PointSet::removePoint(const Point& point) {
	PointIter pointIter = getPointIter(&point);
	assert(pointIter != points.end());
	points.erase(pointIter);

	if(&point == referencePoint) {		// search for new reference point
		referencePoint = nullptr;
		for(const Point* point : points) {
			updateReferencePoint(*point);
		}
		return true;
	}
	return false;
}

// to work this function would have to update the reference point
//bool PointSet::addPoints(std::vector<const Point*> points) {
//	points.insert(std::end(this->points), std::begin(points), std::end(points));
//}

void PointSet::clear() {
	points.clear();
	referencePoint = nullptr;
}

void PointSet::sortPoints(PointComperator comperator) {
	std::sort(points.begin(), points.end(), comperator);
}

void PointSet::sortPointsByAngle() {
	if (points.size() == 0){ return; }
	Vec2f ref = referencePoint->pos();

	std::sort(points.begin(), points.end(), [ref](const Point* a, const Point* b) {
		float angA = getAngle(a->pos() - ref);
		float angB = getAngle(b->pos() - ref);

		assert(angA <= deg2rad(180) + EPS);		// Since the reference point is the bottom most point, angles cant be bigger than 180
		assert(angB <= deg2rad(180) + EPS);		// Since the reference point is the bottom most point, angles cant be bigger than 180

		if(angA == angB) {							// The points are colinear (same angle) --> the point that is nearer to the reference should come first
			float distA = squareDistance(ref, a->pos());
			float distB = squareDistance(ref, b->pos());
			return distA < distB;
		}

		return angA < angB;
	});

	assert(points.front() == referencePoint);
}



int PointSet::getSize() const {
	return points.size();
}

const Point* PointSet::getPoint(int index) const {
	assert(index >= 0 && index < static_cast<int>(points.size()));
	return points[index];
}

const Point* PointSet::getLastPoint() const {
	return points.back();
}

const Point* PointSet::getReferencePoint() const {
	return referencePoint;
}


std::string PointSet::String() const {
	std::stringstream ss;

	ss << points.size() << " Points" << std::endl;
	int idx = 0;
	for(const Point* p : points) {
		ss << "  (" << std::setfill(' ') << std::setw(3) << std::right << idx << ") ";
		ss << std::setfill(' ') << std::setw(8) << std::right << p->x;
		ss << " x ";
		ss << std::setfill(' ') << std::setw(8) << std::left << p->y;

		if(p == referencePoint) {
			ss << "  <reference>";
		}

		ss << std::endl;
		++idx;
	}

	return ss.str();
}

