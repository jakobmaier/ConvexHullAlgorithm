#pragma once
#include "ConvexHull.h"
class PointSet;
class ConvexHull;

class HullFinder {
	const PointSet* pointSet;
	ConvexHull* hull;

	const Point* getPivot() const;
	void findConvexHull();
public:
	explicit HullFinder(const PointSet& input);

	const ConvexHull* getHull();

	~HullFinder();
};

