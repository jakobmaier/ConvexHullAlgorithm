#pragma once
class PointSet;
class ConvexHull;

enum HullAlgoState { SORT_DONE, CANDIDATE_ADDED, CANDIDATE_POPED, FINISHED };

struct HullState {
  HullAlgoState state = SORT_DONE;
  std::stack<const Point *> candiates;
  const Point *pointThatCausedPop = nullptr;
  ConvexHull hull;
  int step = 0;

  static HullState CandidateAdded(std::stack<const Point *> candiates, int step);
  static HullState CandidatePoped(std::stack<const Point *> candiates,
	  const Point *point, int step);
  static HullState SortDone( int step);
  static HullState HullFound(ConvexHull hull);
private:
  HullState() = default;
};

ConvexHull findConvexHull(PointSet *pointSet);
HullState findConvexHullStep(PointSet *pointSet, int simulateUntilStep);