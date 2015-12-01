#include "ConvexHull.h"
#include "HullFinder.h"
#include "Point.h"
#include "PointSet.h"
#include "utils.h"
#include <assert.h>
//#include <iostream>

const float EPS = 0.00001f;

ConvexHull findConvexHull(PointSet *pointSet) {

  int pointCount = pointSet->getSize();

  pointSet->sortPointsByAngle();

  if (pointCount <= 3) { // The points are already a convex hull
    ConvexHull hull;
    for (int i = 0; i < pointCount; ++i) {
      hull.addPoint(*pointSet->getPoint(i));
    }
    if (pointCount > 0) {
      hull.addPoint(*pointSet->getPoint(0));
    }
    return hull;
  }

  std::stack<const Point *> candiates;

  const Point *prev;
  const Point *now;

  candiates.push(pointSet->getLastPoint());
  candiates.push(pointSet->getReferencePoint()); // Is always the first (idx 0)
                                                 // element in the point set

  for (int i = 1; i < pointCount;) {
    const Point *point = pointSet->getPoint(i);

    now = candiates.top();
    candiates.pop();
    prev = candiates.top();
    candiates.push(now);

    if (isCCW(prev->pos(), now->pos(), point->pos())) {
      candiates.push(point);
      // std::cout << "adding " << i << std::endl;
      ++i;
    } else {
      // std::cout << "pop" << std::endl;
      candiates.pop();
    }
  }

  ConvexHull hull(candiates);
  return hull;
}



HullState findConvexHullStep(PointSet *pointSet, int simulateUntilStep) {

  int step = 0;

  int pointCount = pointSet->getSize();

  pointSet->sortPointsByAngle();

  //++step;
  if (step++ == simulateUntilStep) // sort done -> update numbers
  {
    return HullState::SortDone(step);
  }

  if (pointCount <= 3) { // The points are already a convex hull
    ConvexHull hull;
    for (int i = 0; i < pointCount; ++i) {
      hull.addPoint(*pointSet->getPoint(i));
    }
	if (pointCount > 0) {
		hull.addPoint(*pointSet->getPoint(0));
	}
    return HullState::HullFound(hull);
  }

  std::stack<const Point *> candiates;

  const Point *prev;
  const Point *now;

  candiates.push(pointSet->getLastPoint());
  candiates.push(pointSet->getReferencePoint()); // Is always the first (idx 0)
                                                 // element in the point set

  //++step;
  if (step++ == simulateUntilStep) // break print candidates
  {
    return HullState::CandidateAdded(candiates, step);
  }

  for (int i = 1; i < pointCount;) {
    const Point *point = pointSet->getPoint(i);

    now = candiates.top();
    candiates.pop();
    prev = candiates.top();
    candiates.push(now);

    if (isCCW(prev->pos(), now->pos(), point->pos())) {
      candiates.push(point);
      // std::cout << "adding " << i << std::endl;
      ++i;

      //++step;
      if (step++ == simulateUntilStep) // break print candidates
      {
        return HullState::CandidateAdded(candiates, step);
      }
    } else {
      //++step;
      if (step++ == simulateUntilStep) // break print candidates
      {
        return HullState::CandidatePoped(candiates, point, step);
      }
      // std::cout << "pop" << std::endl;
      candiates.pop();
    }
  }
  ConvexHull hull(candiates);
  return HullState::HullFound(hull);
}

// Hull state
HullState HullState::CandidateAdded(std::stack<const Point *> candiates,
                                    int step) {
  HullState state;
  state.state = CANDIDATE_ADDED;
  state.candiates = candiates;
  state.step = step;
  return state;
}

HullState HullState::CandidatePoped(std::stack<const Point *> candiates,
                                    const Point *point, int step) {
  HullState state;
  state.state = CANDIDATE_POPED;
  state.candiates = candiates;
  state.pointThatCausedPop = point;
  state.step = step;
  return state;
}

HullState HullState::SortDone(int step) {
  HullState state;
  state.state = SORT_DONE;
  state.step = step;
  return state;
}

HullState HullState::HullFound(ConvexHull hull) {
  HullState state;
  state.hull = hull;
  state.state = FINISHED;
  return state;
}

// HullState::HullState()
//{
//}