#pragma once
#include "basics.h"
#include "Point.h"

class Dots {
	typedef std::vector<const Point*>::iterator DotIter;
	std::vector<const Point*> dots;

	DotIter getDotIter(const Point* position);
public:
	Dots();

	void addDot(const Point& dot);
	void removeDot(const Point& dot);

	void addDots(std::vector<const Point*> dots);
	void clear();

	int getDotCount() const;
	const Point* getDot(int index) const;

	~Dots();
};

