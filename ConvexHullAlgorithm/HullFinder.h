#pragma once
#include "ConvexHull.h"
class PointSet;
class ConvexHull;

class HullFinder {
	PointSet* pointSet;
	ConvexHull* hull;

	void findConvexHull();
public:
	explicit HullFinder(PointSet& input);

	const ConvexHull* getHull();

	~HullFinder();
};

