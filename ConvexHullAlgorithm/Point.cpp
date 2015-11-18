#include "Point.h"


Point::Point()
: x(0)
, y(0) {
}


Point::Point(float x, float y)
: x(x)
, y(y) {
}

Point::Point(Vec2f position)
: x(position.x)
, y(position.y) {
}

Vec2f Point::pos() const {
	return Vec2f(x, y);
}