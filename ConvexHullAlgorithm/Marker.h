#pragma once
#include <SFML/Graphics.hpp>
#include "DrawableGroup.h"
#include "basics.h"


class Marker {
	Vec2f position;
	float orientationInDegrees;

	float radius;
	float outlineThickness;
	int sides;
	sf::Color color;
	float alpha;			// Stored in addition to color to get a higher presicion (color uses int only)
	bool drawDirectionIndicator;


	DrawableGroup drawableCollection;
	sf::CircleShape* markerOutline;
	sf::RectangleShape* directionIndicator;


	void initialiseDrawableCollection();

	void initialiseMarkerOutline();
	void updateMarkerOutline();

	void initialiseDirectionIndicator();
	void updateDirectionIndicator();
public:
	Marker();
	Marker(Vec2f position, float radius, float orientationInDegrees = 0);
	Marker& operator=(const Marker& other);
	
	void setMarker(Vec2f position, float orientationInDegrees);

	void setPosition(Vec2f position);
	void setRotation(float orientationInDegrees);
	void translate(Vec2f direction);
	void rotate(float angleInDegrees);

	Vec2f getPosition() const;
	float getOrientationInDegrees() const;


	void setAppearance(float radius, float outlineThickness, int sides, Color color);
	void setRadius(float radius);
	void setColor(Color color);
	void setAlpha(float alpha);
	float getAlpha() const;
	float getRadius() const;
	float getOutlineThickness() const;
	int getSides() const;
	Color getColor() const;
	void enableDirectionIndicatorDrawing(bool enable);

	const sf::Drawable* getDrawable() const;
	~Marker();
};

