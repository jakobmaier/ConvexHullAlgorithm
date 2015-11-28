#define _USE_MATH_DEFINES
#include <cmath>

#include "PointSet.h"

#include <assert.h>
#include "Point.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "utils.h"
#include <stack>
#include "ConvexHull.h"

const float EPS = 0.00001f;


PointData::PointData(const Point& point) {
	this->point = &point;
	angleToReference = 0;
}




PointSet::PointSet()
: referencePoint(nullptr)
, anglesToReferenceAreUp2Date(true) {
}



PointSet::~PointSet() {
	clear();
}



PointSet::PointIter PointSet::getPointIter(const Point* position) {
	for(PointIter point = points.begin(); point != points.end(); ++point) {
		const PointData* p = *point;
		if(p->point == position) {
			return point;
		}
	}
	return points.end();
}

PointSet::PointIter PointSet::getPointIter(PointData* position) {
	for(PointIter point = points.begin(); point != points.end(); ++point) {
		if(*point == position) {
			return point;
		}
	}
	return points.end();
}

void PointSet::updateReferencePoint(PointData& newPoint) {
	assert(newPoint.point != nullptr);
	if(referencePoint == nullptr) {
		referencePoint = &newPoint;
		if(points.size() == 0) {
			updateAngleToReferencePoint(referencePoint);
			anglesToReferenceAreUp2Date = true;
		} else {
			anglesToReferenceAreUp2Date = false;
		}
		return;
	}
	if((referencePoint->point->y < newPoint.point->y) ||
	   (referencePoint->point->y == newPoint.point->y && referencePoint->point->x < newPoint.point->x)) {
		referencePoint = &newPoint;
		anglesToReferenceAreUp2Date = false;
	}
}



void PointSet::updateAngleToReferencePoint(PointData* point) {
	Vec2f ref = referencePoint->point->pos();
	Vec2f pos = point->point->pos();
	point->angleToReference = normaliseAngle(atan2f(pos.y - ref.y, pos.x - ref.x));
	assert(point->angleToReference < deg2rad(360));

	if(point == referencePoint) {
		assert(abs(referencePoint->angleToReference) < EPS);
		referencePoint->angleToReference = 0;
	} else {
		assert(point->angleToReference >= deg2rad(180) - EPS);		// Since the reference point is the bottom most point, angles cant be lower than 180
	}
}

void PointSet::updateAnglesToReferencePoint() {
	std::cout << "Updating all angles to reference point" << std::endl;

	// I didn't call "updateAngleToReferencePoint()" to reduce memory access 
	Vec2f ref = referencePoint->point->pos();
	for(PointData* p : points) {
		Vec2f pos = p->point->pos();
		float angle = atan2f(pos.y - ref.y, pos.x - ref.x);
		p->angleToReference = normaliseAngle(angle);
		assert(p->angleToReference < deg2rad(360));
		assert(p->angleToReference >= deg2rad(180) || p == referencePoint);		// Since the reference point is the bottom most point, angles cant be lower than 180
	}
	assert(abs(referencePoint->angleToReference) < EPS);
	referencePoint->angleToReference = 0;
	anglesToReferenceAreUp2Date = true;
}

void PointSet::addPoint(const Point& point) {
	PointData* data = new PointData(point);
	points.push_back(data);
	updateReferencePoint(*data);
	if(anglesToReferenceAreUp2Date) {
		updateAngleToReferencePoint(data);
	}
}

void PointSet::removePoint(const Point& point) {
	PointIter pointIter = getPointIter(&point);
	assert(pointIter != points.end());
	const PointData* p = *pointIter;

	points.erase(pointIter);

	if(p == referencePoint) {		// Completly search for new reference point
		referencePoint = nullptr;
		anglesToReferenceAreUp2Date = false;
		for(PointData* point : points) {
			updateReferencePoint(*point);
		}
	}
}

void PointSet::addPoints(std::vector<const Point*> points) {
	for(const Point* p : points) {
		addPoint(*p);
	}
}

void PointSet::clear() {
	for(const PointData* p : points) {
		delete p;
	}
	points.clear();
	referencePoint = nullptr;
	anglesToReferenceAreUp2Date = true;
}

void PointSet::sortPointsByAngle() {
	forceAnglesToBeUp2Date();

	Vec2f refPos = referencePoint->point->pos();

	std::sort(points.begin(), points.end(),
			  [refPos](PointData* a, PointData* b) {

		if(a->angleToReference == b->angleToReference) {				// The points are colinear (same angle) --> the point that is nearer to the reference should come first
			float distA = squareDistance(refPos, a->point->pos());
			float distB = squareDistance(refPos, b->point->pos());
			return distA < distB;
		}

		return a->angleToReference < b->angleToReference;
	}
	);

	assert(points.front() == referencePoint);
}


void PointSet::sortPoints(PointComperator comperator) {
	std::sort(points.begin(), points.end(),
			  [comperator](PointData* a, PointData* b) {
		return comperator(*a->point, *b->point);
	}
	);
}

int PointSet::getSize() const {
	return points.size();
}

const Point* PointSet::getPoint(int index) const {
	assert(index >= 0 && index < static_cast<int>(points.size()));
	return points[index]->point;
}

const Point* PointSet::getReferencePoint() const {
	return referencePoint->point;
}



void PointSet::forceAnglesToBeUp2Date() {
	if(!anglesToReferenceAreUp2Date) {
		updateAnglesToReferencePoint();
	}
}

std::string PointSet::String() const {
	std::stringstream ss;

	ss << points.size() << " Points" << std::endl;
	int idx = 0;
	for(PointData* p : points) {
		ss << "  (" << std::setfill(' ') << std::setw(3) << std::right << idx << ") ";
		ss << std::setfill(' ') << std::setw(8) << std::right << p->point->x;
		ss << " x ";
		ss << std::setfill(' ') << std::setw(8) << std::left << p->point->y;
		if(anglesToReferenceAreUp2Date) {
			ss << "    angle: ";
			ss << std::setfill(' ') << std::setw(8) << std::left << (p->angleToReference * 180 / M_PI);
		}
		ss << std::endl;
		++idx;
	}

	return ss.str();
}







bool isCCW(PointData* start, PointData* endA, PointData* endB) {
	return isCCW(start->point->pos(), endA->point->pos(), endB->point->pos());
}



void PointSet::findConvexHull() {
	if(points.size() <= 3) {			// The points are already a convex hull
		return;
	}

	std::stack<PointData*> candiates;

	sortPointsByAngle();
	PointData* prev;
	PointData* now;	

	candiates.push(points.back());
	candiates.push(referencePoint);	// Is always the first (idx 0) element in list


	for(int i = 1; i < points.size();) {
		PointData* point = points[i];

		now = candiates.top();
		candiates.pop(); 
		prev = candiates.top();
		candiates.push(now);

		if(isCCW(prev, now, point)) {
			candiates.push(point);
			++i;
		} else {
			candiates.pop();
		}

	}

	ConvexHull hull(candiates);
	while(!candiates.empty()) {
		hull.addPoint(*candiates.top()->point);
		candiates.pop();
	}

	std::cout << hull.String();
	return;

}

