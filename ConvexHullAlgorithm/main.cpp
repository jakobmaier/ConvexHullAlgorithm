#include "Button.h"
#include "Config.h"
#include "ConvexHull.h"
#include "HullFinder.h"
#include "Point.h"
#include "PointSet.h"
#include "utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "Input.h"
#include "LineShape.h"
#include "ResourceManager.h"

std::vector<Button*> g_buttons;
std::vector<sf::CircleShape*> g_dots;
sf::CircleShape *g_reference_dot;
std::vector<LineShape*> g_hull_lines;
std::vector<sf::Text*> g_numbersForDots;

sf::View g_gameView;
sf::View g_sideBarView;
PointSet *g_points = nullptr;
Button g_fieldSizeIndicator;
int g_fieldSize = 500;
int g_numberOfPointsToGen = NR_OF_POINTS_TO_GENERATE;
ResourceManager *g_resManager;

int g_stepCounter = 0;

bool g_showNumbers = true;

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

// create the text lable for a dot when the numbers are sorted
void CreateNumberForDot(sf::Font& font, std::string number, Vec2f pos) {
  sf::Text* text = new sf::Text();
  text->setFont(font);
  text->setCharacterSize(32);
  text->setPosition(pos.x, pos.y - 5);
  text->setColor(DOT_NUMBER_TEXT);
  text->setString(number);
  g_numbersForDots.push_back(text);
}

// creates a dot and alters the color depending on whether the dot is the new
// reference point or not
void CreateDot(Vec2f position) {
  sf::CircleShape* dot = new sf::CircleShape(0);
  dot->setFillColor(DOT_COLOR);
  dot->setRadius(5.f);
  dot->setOrigin(5.f, 5.f);
  dot->setPosition(position);
  g_dots.push_back(dot);
  if (g_points->addPoint(*(new Point(position)))) {
    if (g_reference_dot != nullptr) {
      g_reference_dot->setFillColor(DOT_COLOR);
    }
    g_reference_dot = g_dots.back();
    g_reference_dot->setFillColor(DOT_REFERENCE_COLOR);
  }
  CreateNumberForDot(g_resManager->GetFont(ResourceManager::HACK), std::to_string(g_dots.size() - 1), position);
  g_stepCounter = 0;
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

//######################################################################
Input PollEvents(sf::RenderWindow &window) {
  Input input;

  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  input.isMouseOverSidebar = pixelPos.x <= WINDOW_X * SIDEBAR_X;
  if (input.isMouseOverSidebar) {
    input.mousePos = window.mapPixelToCoords(pixelPos, g_sideBarView);
  } else {
    input.mousePos = window.mapPixelToCoords(pixelPos, g_gameView);
  }

  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Resized) {
      MaintainAspectRatio(window);
    }

    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        input.leftMouseClicked = true;

        if (!input.isMouseOverSidebar) {
          CreateDot(input.mousePos); // maybe move out from herer????!!
        }
        /*std::cout << "mouse clicked at: " << input.mousePos.x << " "
        << input.mousePos.y << std::endl;*/
      }
    }

    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyReleased &&
         event.key.code == sf::Keyboard::Escape)) {
      window.close();
    }
  }
  return input;
}


void CreateLine(Vec2f pos1, Vec2f pos2, sf::Color color = LINE_COLOR){
	LineShape* lines_shape = new LineShape(pos1,pos2);
	lines_shape->setThickness(10.f);
	
		lines_shape->setFillColor(color);
	
	g_hull_lines.push_back(lines_shape);
}

void CreateHull(std::stack<const Point*> &points, const Point* point, bool deleteSegment = false)
{
	if (points.size() < 2) {
		std::cout << "not enough g_points to draw" << std::endl;
		return;
	}
	g_hull_lines.clear();

	

	const Point* elem = points.top();
	points.pop();
	
	if (deleteSegment) // 
	{
		CreateLine(point->pos(), elem->pos(), LINE_COLOR_DELETE);
		CreateLine(point->pos(), points.top()->pos(), LINE_COLOR_NEW);
	} 
	
	do{
		const Point* elemNext = points.top();
		points.pop();
		if (deleteSegment)
		{
			CreateLine(elem->pos(), elemNext->pos(), LINE_COLOR_DELETE);

		} else
		{
			CreateLine(elem->pos(), elemNext->pos(), LINE_COLOR);

		}
		deleteSegment = false;
		elem = elemNext;
	} while (!points.empty());
}


void CreateHull(std::vector< const Point*> &points){
	if (points.size() < 2) {
		std::cout << "not enough g_points to draw" << std::endl;
		return;
	}
	g_hull_lines.clear();
	auto it = points.begin();
	auto itNext = points.begin() + 1;

	while (itNext != points.end()) {
		CreateLine( (*it)->pos(), (*itNext)->pos());
		++it;
		++itNext;
	}
}


void UpdateFieldSize(int delta) {
  g_fieldSize += delta;
  g_fieldSize = (g_fieldSize <= 100) ? 100 : g_fieldSize;

  g_gameView = CreateView(
      sf::Vector2u(g_fieldSize,
                   static_cast<unsigned int>(g_fieldSize * GAME_RATIO)),
      false);
  g_fieldSizeIndicator.SetNumberText(g_fieldSize);
}

void SetupMenu(ResourceManager &resMan) {
  Button* increaseField = new Button(resMan, "+500", sf::Vector2f(195.f, 140.f),
                       sf::Vector2f(100.f, 50.f));
  increaseField->setTriggerFunction([]() { UpdateFieldSize(500); });
  g_buttons.push_back(increaseField);

  Button* decreaseField = new Button(resMan, "-500", sf::Vector2f(305, 140.f),
                       sf::Vector2f(100.f, 50.f));
  decreaseField->setTriggerFunction([]() { UpdateFieldSize(-500); });
  g_buttons.push_back(decreaseField);

  Button* stepHull = new Button(resMan, "Step", sf::Vector2f(180, 200));

  stepHull->setTriggerFunction([]() {

	  std::cout << g_stepCounter << std::endl;

    HullState state = findConvexHullStep(g_points, g_stepCounter);
    g_stepCounter = state.step;
    switch (state.state) {
    case SORT_DONE: {
      std::cout << "Sort done." << std::endl;
	  g_numbersForDots.clear();
	  g_hull_lines.clear();
      int counter = 0;
      for (const Point *point : g_points->points) {
        CreateNumberForDot(g_resManager->GetFont(ResourceManager::HACK),
                           std::to_string(counter), point->pos());
        ++counter;
      }
    } break;
    case CANDIDATE_ADDED:
		CreateHull(state.candiates,nullptr);
		std::cout << "Candidate(s) added." << std::endl;
      //
      break;
    case CANDIDATE_POPED:

		CreateHull(state.candiates,state.pointThatCausedPop, true);

		std::cout << "Candidate poped." << std::endl;

      break;

    case FINISHED:
		std::cout << "Algorithm finished." << std::endl;
      // when finished and this is set to zero the algo could start all over
      // again -> maybe wanted behaviour
      g_stepCounter = 0;
      break;
    default:
      break;
    }
  });
  g_buttons.push_back(stepHull);

  // Button stepHull(resMan, "Step", sf::Vector2f(180, 200));
  // stepHull.setTriggerFunction([]()
  //{

  //});
  // g_buttons.push_back(stepHull);

  Button* calcHull = new Button(resMan, "Calc Hull", sf::Vector2f(180, 260));
  calcHull->setTriggerFunction([]() {
    ConvexHull hull = findConvexHull(g_points);
	CreateHull(hull.points);
    
    // std::cout << hull.String();
  });
  g_buttons.push_back(calcHull);

  Button* genDots = new Button(resMan, "Gen Dots", sf::Vector2f(180, 320));
  genDots->setTriggerFunction([]() {
    for (int i = 0; i < g_numberOfPointsToGen; ++i) {
      CreateDot(Vec2f(random(0, static_cast<float>(g_fieldSize)),
                      random(0, static_cast<float>(g_fieldSize))));
    }
  });
  g_buttons.push_back(genDots);

  Button *clear = new Button(resMan, "Clear", sf::Vector2f(180, 380));
  clear->setTriggerFunction([]() {
    g_points->clear();
    g_dots.clear();
    g_hull_lines.clear();
    g_reference_dot = nullptr;
	g_numbersForDots.clear();

  });
  g_buttons.push_back(clear);
}

//############################################################################
int main() {
  g_dots.reserve(0);

  srand(static_cast<unsigned int>(time(nullptr)));

  // fps timer
  float elapsedTime;
  float windowTitleCounter = 0;
  sf::Clock clock;

  // Globals
  ResourceManager resMan;
  g_resManager = &resMan;
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE,
                          sf::Style::Default, settings);
  g_sideBarView = CreateView(sf::Vector2u(WINDOW_X, WINDOW_Y), true);
  g_gameView = CreateView(sf::Vector2u(1600, 1200), false);

  g_fieldSizeIndicator =
      Button(resMan, "XXX", sf::Vector2f(70, 140), sf::Vector2f(130.f, 50.f));
  g_fieldSizeIndicator.SetDefaultColor(BTN_DEFAULT_INDICATOR_COLOR);
  g_fieldSizeIndicator.Update(Input());
  SetupMenu(resMan);

  UpdateFieldSize(0);

  g_points = new PointSet();

  sf::RectangleShape menuBackground(sf::Vector2f(WINDOW_X, WINDOW_Y));
  menuBackground.setFillColor(sf::Color(100, 100, 100, 100));

  clock.restart();
  while (window.isOpen()) {

    window.clear(CLEAR_COLOR);
    elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    windowTitleCounter += elapsedTime;
    float fps = 1.f / elapsedTime;

    Input input = PollEvents(window);

    // update
    if (input.isMouseOverSidebar) {
      for (auto &button : g_buttons) {
        button->Update(input);
      }
    }

    // draw the g_points
    // ########################################################
    window.setView(g_gameView);
    // SetViewport(window, sf::Vector2u(1600, 1200), false); // todo set to min
    // and max of the range for the g_points
    for (auto &line : g_hull_lines) {
      window.draw(*line);
    }

    for (auto &point : g_dots) {
      window.draw(*point);
    }

	//if (g_numbersForDots.size() > 0 )
	//window.draw(g_numbersForDots[0]);
    for (auto &text : g_numbersForDots) {
		/*text.setFont(resMan.GetFont(ResourceManager::HACK));*/
		window.draw(*text);
    }

    // Draw the menu here #####################################################
    window.setView(g_sideBarView);
    // SetViewport(window, sf::Vector2u(WINDOW_X, WINDOW_Y), true);
    window.draw(menuBackground);
    for (auto &button : g_buttons) {
      button->Draw(window);
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

  // delete g_points;
  return 0;
}