#pragma once
#include "basics.h"

float normaliseAngle(float angleInRad);
float clamp(float value, float min, float max);

float squareDistance(Vec2f from, Vec2f to);
float distance(Vec2f from, Vec2f to);

bool equals(float a, float b, float eps);
bool equals(Vec2f a, Vec2f b, float eps);

float random(float min, float max);

float rad2deg(float angleInRad);
float deg2rad(float angleInDeg);


bool collinear(Vec2f a, Vec2f b, Vec2f c);				// returns true if all points are on a straight line
bool isCCW(Vec2f start, Vec2f endA, Vec2f endB);
bool isLeftOfLine(Vec2f start, Vec2f end, Vec2f other);
float getAngle(Vec2f vector);