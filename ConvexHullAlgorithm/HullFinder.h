#pragma once
class PointSet;
class ConvexHull;

enum HullAlgoState { SORT_DONE, CANDIDATE_ADDED, CANDIDATE_POPED, FINISHED };

struct HullState {
  HullAlgoState state = SORT_DONE;
  std::stack<const Point *> candiates;
  const Point *pointThatCausedPop = nullptr;
  ConvexHull hull;

  static HullState CandidateAdded(std::stack<const Point *> candiates);
  static HullState CandidatePoped(std::stack<const Point *> candiates,
                                  const Point *point);
  static HullState SortDone();
  static HullState HullFound(ConvexHull hull);
private:
  HullState() = default;
};

ConvexHull findConvexHull(PointSet *pointSet);
HullState findConvexHullStep(PointSet *pointSet, int simulateUntilStep);