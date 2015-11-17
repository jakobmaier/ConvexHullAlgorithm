#include <SFML/Graphics.hpp>
#include "Dots.h"
#include <iostream>

int main() {
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


	Dots* dots = new Dots();
	for(int i = 0; i < 100; ++i)
	{
		dots->addDot(Vec2f(i, i));
		dots->removeDot(Vec2f(i-1,i-1));
	}
	std::cout << dots->getDotCount() << std::endl;


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


	delete dots;
	return 0;
}