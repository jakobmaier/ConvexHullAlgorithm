#include "HullFinder.h"
#include "PointSet.h"
#include "Point.h"

//
//const Point* HullFinder::getPivot() const {
//	// Returns the bottommost and rightmost point
//	int pointCount = pointSet->getSize();
//	if(pointCount == 0) {
//		return nullptr;
//	}
//	const Point* pivot = pointSet->getPoint(0);
//	for(int i = 1; i < pointCount; ++i) {
//		const Point* other = pointSet->getPoint(i);
//		if(other->y > pivot->y) {
//			pivot = other;
//		}
//		if(other->y == pivot->y && other->x > pivot->x) {
//			pivot = other;
//		}
//	}
//	return pivot;
//}

void HullFinder::findConvexHull() {
	delete hull;
	hull = new ConvexHull();



	pointSet->sortPointsByAngle();


	//std::vector<point> CH(std::vector<point> P) {
	//	int i, N = (int)P.size();
	//	if(N <= 3) return P; // special case, the CH is P itself
	//	// first, find P0 = point with lowest Y and if tie: rightmost X
	//	int P0 = 0;
	//	for(i = 1; i < N; i++)
	//	if(P[i].y < P[P0].y || P[i].y == P[P0].y && P[i].x > P[P0].x))
	//		P0 = i;
	//	// swap selected vertex with P[0]
	//	point temp = P[0]; P[0] = P[P0]; P[P0] = temp;
	//	// second, sort points by angle w.r.t. pivot P0
	//	pivot = P[0]; // use this global variable as reference
	//	sort(++P.begin(), P.end(), angleCmp); // notice that we does not sort P[0]
	//	// third, the ccw tests
	//	point prev(0, 0), now(0, 0);
	//	stack<point> S; S.push(P[N - 1]); S.push(P[0]); // initial content of stack S
	//	i = 1; // then, we check the rest
	//	while(i < N) { // note: N must be >= 3 for this method to work
	//		now = S.top();
	//		S.pop(); prev = S.top(); S.push(now); // get 2nd from top
	//		if(ccw(prev, now, P[i])) S.push(P[i++]); // left turn, accept
	//		else S.pop(); // otherwise, pop the top of stack S until we have a left turn
	//	}
	//	vector<point> ConvexHull; // from stack back to vector
	//	while(!S.empty()) { ConvexHull.push_back(S.top()); S.pop(); }
	//	return ConvexHull;
	// } // return the resul





}













HullFinder::HullFinder(PointSet& input)
: pointSet(&input)
, hull(nullptr) {
}

const ConvexHull* HullFinder::getHull() {
	if(hull == nullptr) {
		findConvexHull();
	}
	return hull;
}

HullFinder::~HullFinder() {
	delete hull;
}
