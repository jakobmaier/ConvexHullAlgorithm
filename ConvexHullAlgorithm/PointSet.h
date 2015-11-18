#pragma once
#include <vector>

struct Point;

class PointSet {
	typedef std::vector<const Point*>::iterator PointIter;
	std::vector<const Point*> points;

	PointIter getPointIter(const Point* position);
public:
	PointSet();

	void addPoint(const Point& point);
	void removePoint(const Point& point);

	void addPoints(std::vector<const Point*> points);
	void clear();

	int getSize() const;
	const Point* getPoint(int index) const;

	~PointSet();
};

