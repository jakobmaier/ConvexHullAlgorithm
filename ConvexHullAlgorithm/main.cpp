#include "Config.h"
#include "ConvexHull.h"
#include "HullFinder.h"
#include "Point.h"
#include "PointSet.h"
#include "utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#include "Button.h"

#include "Input.h"
#include "ResourceManager.h"

// set the viewport to a given size to allow the window to scale
void SetViewport(sf::RenderWindow &window, sf::Vector2u size, bool setForMenu) {

	float sidebarWidth = 0.3f;
  if (setForMenu)
  {
	  sf::View view(sf::FloatRect(0, 0, size.x * sidebarWidth, size.y));
	  view.setViewport(sf::FloatRect(0, 0, sidebarWidth, 1));
	  window.setView(view);
  } else
  {
	  sf::View view(sf::FloatRect(0, 0, size.x, size.y));
	  view.setViewport(sf::FloatRect(sidebarWidth, 0,1-sidebarWidth, 1));
	  window.setView(view);
  }


}

Input PollEvents(sf::RenderWindow &window) {
  Input input;

  input.mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  sf::Event event;
  while (window.pollEvent(event)) {

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        input.leftMouseClicked = true;
        /*std::cout << "mouse clicked at: " << input.mousePos.x << " "
                << input.mousePos.y << std::endl;*/
      }
    }

    if (event.type == sf::Event::Closed ||
        event.type == sf::Event::KeyReleased &&
            event.key.code == sf::Keyboard::Escape) {
      window.close();
    }
  }
  return input;
}

int main() {

  srand(static_cast<unsigned int>(time(nullptr)));

  // fps timer
  float elapsedTime;
  float windowTitleCounter = 0;
  sf::Clock clock;

  // Globals
  ResourceManager resMan;
  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE);

  // TODO delme
  Button test(resMan, "T45   dfdf", sf::Vector2f(180, 200));
  test.setTriggerFunction([]() { std::cout << "fuck off" << std::endl; });

  // set points range and adjust the viewpoint
  // -> range plus border of the add point function

  // PointSet *points = new PointSet();

  // points->addPoint(*(new Point(10, 10)));
  // points->addPoint(*(new Point(0, 0)));
  // points->addPoint(*(new Point(10, 0)));
  // points->addPoint(*(new Point(0, 10)));
  // points->addPoint(*(new Point(24, 24)));

  // for (int i = 0; i < 100; ++i) {
  //  // points->addPoint(*(new Point(static_cast<float>(i),
  //  // static_cast<float>(i))));
  //  points->addPoint(*new Point(random(-100, +100), random(-100, +100)));
  //}
  // std::cout << points->String() << std::endl;

  // ConvexHull hull = findConvexHull(points);
  // std::cout << hull.String();

  sf::RectangleShape menuBackground(sf::Vector2f(WINDOW_X, WINDOW_Y));
  menuBackground.setFillColor(sf::Color(100,100,100,100));


  sf::RectangleShape rect(sf::Vector2f(800.f, 600.f));
  rect.setFillColor(sf::Color::Blue);

  clock.restart();
  while (window.isOpen()) {

    window.clear(sf::Color::White);
    elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    windowTitleCounter += elapsedTime;
    float fps = 1.f / elapsedTime;

    Input input = PollEvents(window);

    // update
    test.Update(input);
    // test.events(input);

    // draw the points

    SetViewport(window, sf::Vector2u(1600, 1200),false);

    window.draw(rect);

    // Draw the menu here
    SetViewport(window, sf::Vector2u(WINDOW_X, WINDOW_Y),true);

	window.draw(menuBackground);

    // window.draw(test);
    test.Draw(window);

    // update title
    if (windowTitleCounter > 0.75) {
      windowTitleCounter = 0;
      std::ostringstream ss;
      ss.precision(4);
      ss << WINDOW_TITLE << fps << " fps";
      window.setTitle(ss.str());
    }
    window.display();
  }

  // delete points;
  return 0;
}