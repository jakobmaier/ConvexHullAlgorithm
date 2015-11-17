#include "Dots.h"


Dots::Dots() {
}


Dots::~Dots() {
}

Dots::DotIter Dots::getDotIter(Vec2f position) {
	for(DotIter dot = dots.begin(); dot != dots.end(); ++dot) {
		if(*dot == position) {
			return dot;
		}
	}
	return dots.end();
}

void Dots::addDot(Vec2f position) {
	dots.push_back(position);
}

void Dots::removeDot(Vec2f position) {
	DotIter dot = getDotIter(position);
	if(dot != dots.end()) {
		dots.erase(dot);
	}
}

void Dots::addDots(std::vector<Vec2f> dots) {
	dots.insert(std::end(this->dots), std::begin(dots), std::end(dots));
}

void Dots::clear() {
	dots.clear();
}

int Dots::getDotCount() const {
	return dots.size();
}

Vec2f Dots::getDot(int index) const {
	return dots[index];
}
