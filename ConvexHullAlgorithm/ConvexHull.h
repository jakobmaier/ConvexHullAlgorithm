#pragma once
#include <vector>
#include <stack>
struct Point;

class ConvexHull {
	typedef std::vector<const Point*>::iterator PointIter;
	typedef std::vector<const Point*>::const_iterator CPointIter;
	std::vector<const Point*> points;

public:
	ConvexHull();
	void addPoint(const Point& point);
	void removeLastPoint();	
	void clear();

	int getSize() const;
	const Point* getPoint(int index) const;

	bool containsPoint(const Point& point) const;

	~ConvexHull();
};

