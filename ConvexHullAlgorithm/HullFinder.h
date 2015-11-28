#pragma once
#include "ConvexHull.h"
class PointSet;
class ConvexHull;

class HullFinder {
	const PointSet* pointSet;
	ConvexHull* hull;

	void findConvexHull();
public:
	explicit HullFinder(const PointSet& input);

	const ConvexHull* getHull();

	~HullFinder();
};

