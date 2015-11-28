#include <SFML/Graphics.hpp>
#include "PointSet.h"
#include "Point.h"
#include <iostream>
#include "utils.h"
#include "HullFinder.h"
#include "ConvexHull.h"

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


	PointSet* points = new PointSet();

	points->addPoint(*(new Point(10, 10)));
	points->addPoint(*(new Point(0, 0)));
	points->addPoint(*(new Point(10, 0)));
	points->addPoint(*(new Point(0, 10)));
	points->addPoint(*(new Point(24, 24)));

	
	//for(int i = 0; i < 100; ++i)
	//{
	//	//points->addPoint(*(new Point(static_cast<float>(i), static_cast<float>(i))));
	//	points->addPoint(*new Point(random(-100, +100), random(-100, +100) ));
	//}
	std::cout << points->String() << std::endl;;

	ConvexHull hull = findConvexHull(points);
	std::cout << hull.String();

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}


	delete points;
	return 0;
}