#pragma once
#include "basics.h"

class Dots {
	typedef std::vector<Vec2f>::iterator DotIter;
	std::vector<Vec2f> dots;

	DotIter getDotIter(Vec2f position);
public:
	Dots();

	void addDot(Vec2f position);
	void removeDot(Vec2f position);

	void addDots(std::vector<Vec2f> dots);
	void clear();

	int getDotCount() const;
	Vec2f getDot(int index) const;

	~Dots();
};

