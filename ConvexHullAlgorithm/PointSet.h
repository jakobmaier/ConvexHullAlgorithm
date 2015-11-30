#pragma once
#include <vector>

struct Point;

//struct PointData {
//	const Point* point;
//	float angleToReference;
//
//	PointData(const Point& point);
//};

/*
 *  Contains a set of points
 *  Mantains a reference point (buttom and right most point in set)
 */
class PointSet {
	typedef std::vector<const Point*>::iterator PointIter;
	std::vector<const Point*> points;
	const Point* referencePoint;				// bottom/right-most point; Is updated everytime the pointset is modified
	
	PointIter getPointIter(const Point* point);

	bool updateReferencePoint(const Point& newPoint);

public:
	typedef bool PointComperator(const Point* a, const Point* b);

	PointSet();

	bool addPoint(const Point& point);
	bool removePoint(const Point& point);

	//bool addPoints(std::vector<const Point*> points);
	void clear();

	void sortPoints(PointComperator comperator);
	void PointSet::sortPointsByAngle();

	int getSize() const;
	const Point* getPoint(int index) const;
	const Point* getLastPoint() const;
	const Point* getReferencePoint() const;


	void findConvexHull();


	std::string String() const;

	~PointSet();
};

