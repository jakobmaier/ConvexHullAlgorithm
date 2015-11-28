#include "HullFinder.h"
#include "PointSet.h"
#include "Point.h"
#include <assert.h>
#include "utils.h"
#include <iostream>
#include "ConvexHull.h"


const float EPS = 0.00001f;





ConvexHull findConvexHull(PointSet* pointSet) {
	int pointCount = pointSet->getSize();

	pointSet->sortPointsByAngle();

	if(pointCount <= 3) {			// The points are already a convex hull
		ConvexHull hull;
		for(int i = 0; i < pointCount; ++i) {
			hull.addPoint(*pointSet->getPoint(i));
		}
		return hull;
	}



	std::stack<const Point*> candiates;
	

	const Point* prev;
	const Point* now;

	candiates.push(pointSet->getLastPoint());
	candiates.push(pointSet->getReferencePoint());	// Is always the first (idx 0) element in the point set


	for(int i = 1; i < pointCount;) {
		const Point* point = pointSet->getPoint(i);

		now = candiates.top();
		candiates.pop();
		prev = candiates.top();
		candiates.push(now);

		if(isCCW(prev->pos(), now->pos(), point->pos())) {
			candiates.push(point);
			std::cout << "adding " << i << std::endl;
			++i;
		} else {
			std::cout << "pop" << std::endl;
			candiates.pop();
		}

	}

	ConvexHull hull(candiates);
	return hull;
}

