#pragma once
#include <vector>
#include <stack>
struct Point;

class ConvexHull {
	typedef std::vector<const Point*>::iterator PointIter;
	typedef std::vector<const Point*>::const_iterator CPointIter;

public:
	ConvexHull();
	explicit ConvexHull(std::stack<const Point*> hull);


	void addPoint(const Point& point);
	void clear();

	int getSize() const;
	const Point* getPoint(int index) const;

	bool containsPoint(const Point& point) const;

	~ConvexHull();
	std::string String() const;

	std::vector<const Point*> points;

};

