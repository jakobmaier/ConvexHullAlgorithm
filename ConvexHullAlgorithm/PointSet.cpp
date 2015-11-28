#include "PointSet.h"
#include <assert.h>
#include "Point.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>

const float EPS = 0.00001f;

float normaliseAngle(float angleInRad) {
	float PI_2 = static_cast<float>(2 * M_PI);
	angleInRad += PI_2 *  ceilf(-angleInRad / PI_2);
	angleInRad -= PI_2 * floorf( angleInRad / PI_2);
	return angleInRad;
}



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
	assert(point->angleToReference >= 0 && point->angleToReference < 360);
	if(point == referencePoint) {
		assert(abs(referencePoint->angleToReference) < EPS);
		referencePoint->angleToReference = 0;
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
		assert(p->angleToReference >= 0 && p->angleToReference < 360);
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
		ss << "  (" << std::setfill(' ') << std::setw(3) << idx << ") ";
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

