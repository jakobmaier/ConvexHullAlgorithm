#include "Dots.h"
#include <assert.h>


Dots::Dots() {
}


Dots::~Dots() {
}

Dots::DotIter Dots::getDotIter(const Point* position) {
	for(DotIter dot = dots.begin(); dot != dots.end(); ++dot) {
		if(*dot == position) {
			return dot;
		}
	}
	return dots.end();
}

void Dots::addDot(const Point& dot) {
	dots.push_back(&dot);
}

void Dots::removeDot(const Point& dot) {
	DotIter dotIter = getDotIter(&dot);
	if(dotIter != dots.end()) {
		dots.erase(dotIter);
	}
}

void Dots::addDots(std::vector<const Point*> dots) {
	dots.insert(std::end(this->dots), std::begin(dots), std::end(dots));
}

void Dots::clear() {
	dots.clear();
}

int Dots::getDotCount() const {
	return dots.size();
}

const Point* Dots::getDot(int index) const {
	assert(index >= 0 && index < dots.size());
	return dots[index];
}
