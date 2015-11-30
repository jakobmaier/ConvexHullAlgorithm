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
#include "LineShape.h"

std::vector<Button> buttons;
std::vector<sf::CircleShape> dots;
sf::CircleShape* reference_dot;
std::vector<LineShape> hull_lines;

sf::View gameView;
sf::View sideBarView;
PointSet *points = nullptr;
Button g_fieldSizeIndicator;
int fieldSize = 500;
int g_numberOfPointsToGen = NR_OF_POINTS_TO_GENERATE;

// set the viewport to a given size to allow the window to scale
// this function depends on the window size -> to have correct sized buttons use
// the coordinatespace of the window accordingly
sf::View CreateView(sf::Vector2u size, bool setForMenu) {

  auto sidebarWidth = 0.3f;
  if (setForMenu) {
    sf::View view(sf::FloatRect(0, 0, size.x * sidebarWidth, size.y));
    view.setViewport(sf::FloatRect(0, 0, sidebarWidth, 1));
    return view;
  }
  sf::View view(sf::FloatRect(0, 0, size.x, size.y));
  view.setViewport(sf::FloatRect(sidebarWidth, 0, 1 - sidebarWidth, 1));
  return view;
}

void CreateDot(Vec2f position) {
  sf::CircleShape dot(0);
  dot.setFillColor(DOT_COLOR);
  dot.setRadius(5.f);
  dot.setOrigin(5.f, 5.f);
  dot.setPosition(position);
  dots.push_back(dot);
  if (points->addPoint(*(new Point(position))))
  {
	  if (reference_dot != nullptr)
	  {
		  reference_dot->setFillColor(DOT_COLOR);
	  }
	  reference_dot = &dots.back();
	  reference_dot->setFillColor(DOT_REFERENCE_COLOR);
  }


}

float currentAspectWidth = 0;
float currentAspectHeight = 0;
//void MaintainAspectRatio(sf::RenderWindow& window)
//{
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
//	std::cout << "width: " << currentAspectWidth << " height: " << currentAspectHeight;
//	window.setSize(sf::Vector2u(currentAspectWidth, currentAspectHeight));
//}


//######################################################################
Input PollEvents(sf::RenderWindow &window) {
  Input input;

  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  input.isMouseOverSidebar = pixelPos.x <= WINDOW_X * SIDEBAR_X;
  if (input.isMouseOverSidebar) {
    input.mousePos = window.mapPixelToCoords(pixelPos, sideBarView);
  } else {
    input.mousePos = window.mapPixelToCoords(pixelPos, gameView);
  }

  sf::Event event;
  while (window.pollEvent(event)) {
	  if (event.type == sf::Event::Resized)
	  {
		  //MaintainAspectRatio(window);
	  }

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        input.leftMouseClicked = true;

        if (!input.isMouseOverSidebar) {
          CreateDot(input.mousePos);
        }
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

void UpdateFieldSize(float delta) {
  fieldSize += delta;
  fieldSize = (fieldSize <= 100) ? 100 : fieldSize;

  gameView = CreateView(sf::Vector2u(fieldSize, fieldSize * GAME_RATIO), false);
  g_fieldSizeIndicator.SetNumberText(fieldSize);
}

void SetupMenu(ResourceManager &resMan) {
  Button increaseField(resMan, "+500", sf::Vector2f(195, 140.f),
                       sf::Vector2f(100.f, 50.f));
  increaseField.setTriggerFunction([]() { UpdateFieldSize(500); });
  buttons.push_back(increaseField);

  Button decreaseField(resMan, "-500", sf::Vector2f(305, 140.f),
                       sf::Vector2f(100.f, 50.f));
  decreaseField.setTriggerFunction([]() { UpdateFieldSize(-500); });
  buttons.push_back(decreaseField);

  Button test(resMan, "T45   dfdf", sf::Vector2f(180, 200));
  test.setTriggerFunction([]() { std::cout << "fuck off" << std::endl; });
  buttons.push_back(test);

  Button calcHull(resMan, "Calc. Hull", sf::Vector2f(180, 260));
  calcHull.setTriggerFunction([]() {
    ConvexHull hull = findConvexHull(points);
	if (hull.points.size() < 2)
	{
		std::cout << "not enough points to draw" << std::endl;
		return;
	}
	hull_lines.clear();
	auto it = hull.points.begin();
	auto itNext = hull.points.begin()+1;

	while (itNext != hull.points.end())
	{
		LineShape lines_shape( (*it)->pos() ,(*itNext)->pos());
		lines_shape.setThickness(20.f);
		lines_shape.setFillColor(sf::Color::Black);

		hull_lines.push_back(lines_shape);

		++it;
		++itNext;
	}
    //std::cout << hull.String();
  });
  buttons.push_back(calcHull);

  Button genDots(resMan, "Gen Dots", sf::Vector2f(180, 320));
  genDots.setTriggerFunction([]() {
    for (int i = 0; i < g_numberOfPointsToGen; ++i) {
      CreateDot(Vec2f(random(0, fieldSize), random(0, fieldSize)));
    }
  });
  buttons.push_back(genDots);

  Button clear(resMan, "Clear", sf::Vector2f(180, 380));
  clear.setTriggerFunction([]() {
    points->clear();
    dots.clear();
	hull_lines.clear();
  });
  buttons.push_back(clear);
}

//############################################################################
int main() {
  dots.reserve(500);

  srand(static_cast<unsigned int>(time(nullptr)));

  // fps timer
  float elapsedTime;
  float windowTitleCounter = 0;
  sf::Clock clock;

  // Globals
  ResourceManager resMan;
  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE);
  sideBarView = CreateView(sf::Vector2u(WINDOW_X, WINDOW_Y), true);
  gameView = CreateView(sf::Vector2u(1600, 1200), false);

  g_fieldSizeIndicator = Button(resMan, "XXX", sf::Vector2f(70, 140),
                  sf::Vector2f(130.f, 50.f));
  g_fieldSizeIndicator.SetDefaultColor(BTN_DEFAULT_INDICATOR_COLOR);
  g_fieldSizeIndicator.Update(Input());
  SetupMenu(resMan);

  UpdateFieldSize(0);



  points = new PointSet();

  sf::RectangleShape menuBackground(sf::Vector2f(WINDOW_X, WINDOW_Y));
  menuBackground.setFillColor(sf::Color(100, 100, 100, 100));

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
    if (input.isMouseOverSidebar) {
      for (auto &button : buttons) {
        button.Update(input);
      }
    }

    // draw the points ########################################################
    window.setView(gameView);
    // SetViewport(window, sf::Vector2u(1600, 1200), false); // todo set to min
    // and max of the range for the points
    window.draw(rect);
	for (auto &line : hull_lines)
	{
		window.draw(line);
	}
	
	for (auto &point : dots) {
      window.draw(point);
    }



    // Draw the menu here #####################################################
    window.setView(sideBarView);
    // SetViewport(window, sf::Vector2u(WINDOW_X, WINDOW_Y), true);
    window.draw(menuBackground);
    for (auto &button : buttons) {
      button.Draw(window);
    }
    g_fieldSizeIndicator.Draw(window);

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