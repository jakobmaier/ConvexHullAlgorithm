#pragma once
#include <vector>

struct Point;

struct PointData {
	const Point* point;
	float angleToReference;

	PointData(const Point& point);
};

/*
 *  Contains a set of points that should be connected into a single convex hull
 *  Mantains a reference point (buttom and right most point in set)
 *  Mantains angles from all points to the reference point
 */
class PointSet {
	typedef std::vector<PointData*>::iterator PointIter;
	std::vector<PointData*> points;
	PointData* referencePoint;				// bottom/right-most point; Is updated everytime the pointset is modified
	mutable bool anglesToReferenceAreUp2Date;	

	PointIter getPointIter(const Point* position);
	PointIter getPointIter(PointData* position);

	void updateReferencePoint(PointData& position);

	void updateAngleToReferencePoint(PointData* point);
	void updateAnglesToReferencePoint();

	//typedef bool InternalPointComperator(PointData& a, PointData& b);
public:
	typedef bool PointComperator(const Point& a, const Point& b);

	PointSet();

	void addPoint(const Point& point);
	void removePoint(const Point& point);

	void addPoints(std::vector<const Point*> points);
	void clear();

	void sortPoints(PointComperator comperator);

	int getSize() const;
	const Point* getPoint(int index) const;
	const Point* getReferencePoint() const;

	void forceAnglesToBeUp2Date();
	std::string String() const;

	~PointSet();
};

