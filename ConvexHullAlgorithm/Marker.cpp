#include "Marker.h"
#include <assert.h>


Marker::Marker()
: position({0,0})
, orientationInDegrees(0)
, radius(10)
, outlineThickness(3)
, sides(6)
, color(sf::Color::White)
, drawDirectionIndicator(false)
, markerOutline(nullptr)
, directionIndicator(nullptr) {
	initialiseMarkerOutline();
	initialiseDirectionIndicator();
	initialiseDrawableCollection();
}

Marker::Marker(Vec2f position, float radius, float orientationInDegrees)
: position(position)
, orientationInDegrees(orientationInDegrees)
, radius(radius)
, outlineThickness(3)
, sides(6)
, color(sf::Color::White)
, drawDirectionIndicator(false)
, markerOutline(nullptr)
, directionIndicator(nullptr) {
	initialiseMarkerOutline();
	initialiseDirectionIndicator();
	initialiseDrawableCollection();
}

Marker& Marker::operator=(const Marker& other) {
	this->position = other.position;
	this->orientationInDegrees = other.orientationInDegrees;
	this->radius = other.radius;
	this->outlineThickness = other.outlineThickness;
	this->sides = other.sides;
	this->color = other.color;
	this->drawDirectionIndicator = other.drawDirectionIndicator;
	updateMarkerOutline();
	updateDirectionIndicator();
	return *this;
}


void Marker::initialiseDrawableCollection() {
	drawableCollection.clear();
	drawableCollection.add(*markerOutline);
	drawableCollection.add(*directionIndicator);
}

void Marker::initialiseMarkerOutline() {
	delete markerOutline;
	markerOutline = new sf::CircleShape();
	markerOutline->setFillColor(sf::Color::Transparent);
	updateMarkerOutline();
}

void Marker::updateMarkerOutline() {
	markerOutline->setRadius(radius);
	markerOutline->setOrigin(radius, radius);
	markerOutline->setPointCount(sides);
	markerOutline->setOutlineThickness(outlineThickness);
	markerOutline->setOutlineColor(color);
	markerOutline->setPosition(position);
	markerOutline->setRotation(orientationInDegrees);
}

void Marker::initialiseDirectionIndicator() {
	delete directionIndicator;
	directionIndicator = new sf::RectangleShape();
	updateDirectionIndicator();
}

void Marker::updateDirectionIndicator() {
	directionIndicator->setSize(sf::Vector2f(radius, outlineThickness));

	sf::Vector2f positionOffset(-outlineThickness / 2, -outlineThickness / 2);
	directionIndicator->setPosition(getPosition() + positionOffset);
	directionIndicator->setRotation(orientationInDegrees);
	directionIndicator->setFillColor(color);
}


void Marker::setMarker(Vec2f position, float orientationInDegrees) {
	this->position = position;
	this->orientationInDegrees = orientationInDegrees;
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::setPosition(Vec2f position) {
	this->position = position;
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::setRotation(float orientationInDegrees) {
	this->orientationInDegrees = orientationInDegrees;
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::translate(Vec2f direction) {
	setPosition(position + direction);
}

void Marker::rotate(float angleInDegrees) {
	setRotation(orientationInDegrees + angleInDegrees);
}

float Marker::getOrientationInDegrees() const {
	return orientationInDegrees;
}

void Marker::enableDirectionIndicatorDrawing(bool enable) {
	drawDirectionIndicator = enable;
}

Vec2f Marker::getPosition() const {
	return position;
}


void Marker::setAppearance(float radius, float outlineThickness, int sides, Color color) {
	this->radius = radius;
	this->outlineThickness = outlineThickness;
	this->sides = sides;
	this->color = color;
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::setRadius(float radius) {
	this->radius = radius;
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::setColor(Color color) {
	this->color = color;
	this->alpha = static_cast<float>(color.a);
	updateMarkerOutline();
	updateDirectionIndicator();
}

void Marker::setAlpha(float alpha) {
	this->alpha = alpha;
	this->color.a = static_cast<char>(alpha);
	updateMarkerOutline();
	updateDirectionIndicator();
}

float Marker::getAlpha() const {
	assert(color.a == static_cast<int>(alpha));
	return alpha;
}

float Marker::getOutlineThickness() const {
	return outlineThickness;
}

float Marker::getRadius() const {
	return radius;
}

int Marker::getSides() const {
	return sides;
}

Color Marker::getColor() const {
	return color;
}

const sf::Drawable* Marker::getDrawable() const {
	if(drawDirectionIndicator) {
		return &drawableCollection;
	} else {
		return markerOutline;
	}
}

Marker::~Marker() {
	delete markerOutline;
	delete directionIndicator;
}
