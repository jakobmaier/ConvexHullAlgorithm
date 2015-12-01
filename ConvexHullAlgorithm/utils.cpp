#define _USE_MATH_DEFINES
#include <cmath>

#include "utils.h"

#include "basics.h"
#include <assert.h>
#include <random>

const float EPS = 0.00001f;

const float PI = static_cast<float>(M_PI);
const float PI2 = static_cast<float>(2 * M_PI);

float normaliseAngle(float angleInRad) {
  angleInRad += PI2 * ceilf(-angleInRad / PI2);
  angleInRad -= PI2 * floorf(angleInRad / PI2);
  return angleInRad;
}

float clamp(float value, float min, float max) {
  assert(max >= min);
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}

float squareDistance(Vec2f from, Vec2f to) {
  Vec2f vec = to - from;
  return vec.x * vec.x + vec.y * vec.y;
}

float distance(Vec2f from, Vec2f to) { return sqrtf(squareDistance(from, to)); }

float GetLength(Vec2f vec) { return (vec.x * vec.x) + (vec.y * vec.y); }

bool equals(float a, float b, float eps) {
  assert(eps >= 0);
  if (abs(a - b) <= eps) {
    return true;
  }
  return false;
}

bool equals(Vec2f a, Vec2f b, float eps) {
  return equals(a.x, b.x, eps) && equals(a.y, b.y, eps);
}

float random(float min, float max) {
  assert(min <= max);
  std::random_device rd; // to initialise the random number generator
  std::mt19937 generator(rd());
  std::uniform_real_distribution<float> dist(min, max);
  // static auto randomSource = std::bind(uniform_distribution, generator);
  // float rand = randomSource();
  float rand = dist(generator);
  assert(rand >= min && rand <= max);
  return rand;
}

float rad2deg(float angleInRad) { return angleInRad * 180.f / PI; }

float deg2rad(float angleInDeg) { return angleInDeg * PI / 180.f; }

float cross(Vec2f start, Vec2f endA,
            Vec2f endB) { // cross product of two vectors defined by three
                          // points. The start point is shared
  Vec2f sa = endA - start;
  Vec2f sb = endB - start;
  return sa.x * sb.y - sa.y * sb.x;
}

bool collinear(Vec2f a, Vec2f b, Vec2f c) { return fabs(cross(a, b, c)) < EPS; }

bool isCCW(Vec2f start, Vec2f endA, Vec2f endB) {
  return cross(start, endA, endB) >= 0;
}

bool isLeftOfLine(Vec2f start, Vec2f end, Vec2f other) {
  return isCCW(start, end, other);
}

float getAngle(Vec2f vector) {
  float angle = normaliseAngle(atan2f(vector.y, vector.x));
  assert(angle >= 0);
  assert(angle < deg2rad(360));
  return angle;
}

// set the viewport to a given size to allow the window to scale
// this function depends on the window size -> to have correct sized buttons use
// the coordinatespace of the window accordingly
sf::View CreateView(sf::Vector2u size, bool setForMenu) {

  auto sidebarWidth = 0.3f;
  if (setForMenu) {
    sf::View view(sf::FloatRect(0.f, 0.f,
                                static_cast<float>(size.x) * sidebarWidth,
                                static_cast<float>(size.y)));
    view.setViewport(sf::FloatRect(0.f, 0.f, sidebarWidth, 1.f));
    return view;
  }
  sf::View view(sf::FloatRect(0, 0, static_cast<float>(size.x),
                              static_cast<float>(size.y)));
  view.setViewport(sf::FloatRect(sidebarWidth, 0, 1 - sidebarWidth, 1));
  return view;
}

float currentAspectWidth = 0;
float currentAspectHeight = 0;
void MaintainAspectRatio(sf::RenderWindow &window) {
  // TODO impl
  //	//first we check our new aspect width to see if it changed
  //	float newAspectWidth = window.getSize().x;
  //	float newAspectHeight = window.getSize().y;
  //	if (newAspectWidth != currentAspectWidth)
  //	{
  //		//width changed, maintain the aspect ratio and adjust the height
  //		currentAspectWidth = newAspectWidth;
  //		currentAspectHeight = currentAspectWidth / aspectRatio;
  //	}
  //	else if (newAspectHeight != currentAspectHeight)
  //	{
  //		//height changed, maintain aspect ratio and change the width
  //		currentAspectHeight = newAspectHeight;
  //		currentAspectWidth = currentAspectHeight * aspectRatio;
  //	}
  //	std::cout << "width: " << currentAspectWidth << " height: " <<
  // currentAspectHeight;
  //	window.setSize(sf::Vector2u(currentAspectWidth, currentAspectHeight));
}