#pragma once
#include "basics.h"

struct Point {
	float x;
	float y;


	Point();
	Point(float x, float y);
	Point(Vec2f position);
	Vec2f pos() const;
};

