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

#include "Dots.h"
#include "Hull.h"
#include "PerformanceMonitor.h"

// move into main scope

Hull hull;
Dots *dots;

//#####################

std::vector<Button *> g_buttons;
sf::View g_gameView;
sf::View g_sideBarView;
Button g_fieldSizeIndicator;
int g_fieldSize = 500;
int g_numberOfPointsToGen = NR_OF_POINTS_TO_GENERATE;
ResourceManager *g_resManager;

int g_stepCounter = 0;

bool animate = false;

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

//######################################################################
float GetLength(Vec2f vec) { return (vec.x * vec.x) + (vec.y * vec.y); }

Vec2f g_old_mousePos;
bool rightMousedown = false;
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
    if (event.type == sf::Event::MouseMoved) {

      if (rightMousedown) {

        auto deltaMouse =
            g_old_mousePos - Vec2f(event.mouseMove.x, event.mouseMove.y);

        g_old_mousePos = Vec2f(event.mouseMove.x, event.mouseMove.y);
        float lenght = GetLength(deltaMouse);
        if (lenght >= -45 && lenght <= 45) {
          g_gameView.move(deltaMouse);
        }

        // std::cout << deltaMouse.x << std::endl;
      }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Right) {
        rightMousedown = true;
        g_old_mousePos = Vec2f(event.mouseMove.x, event.mouseMove.y);
      }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
      if (event.mouseButton.button == sf::Mouse::Right) {
        rightMousedown = false;
      }
      if (event.mouseButton.button == sf::Mouse::Left) {
        input.leftMouseClicked = true;

        if (!input.isMouseOverSidebar) {
          dots->CreateDotWithLabelAndPushBack(
              input.mousePos); // maybe move out from herer????!!
          g_stepCounter = 0;
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

void UpdateFieldSize(int delta) {
  g_fieldSize += delta;
  g_fieldSize = (g_fieldSize <= 100) ? 100 : g_fieldSize;

  g_gameView = CreateView(
      sf::Vector2u(g_fieldSize,
                   static_cast<unsigned int>(g_fieldSize * GAME_RATIO)),
      false);
  g_fieldSizeIndicator.SetNumberText(g_fieldSize);
}
bool TakeStep() {

  std::cout << g_stepCounter << std::endl;

  HullState state = findConvexHullStep(dots->m_point_set.get(), g_stepCounter);
  g_stepCounter = state.step;
  switch (state.state) {
  case SORT_DONE: {
    std::cout << "Sort done." << std::endl;
    dots->ClearLabels();
    dots->CreateDotLabels();
    hull.Clear();
  } break;

  case CANDIDATE_ADDED:
    hull.CreateHull(state.candiates);
    std::cout << "Candidate(s) added." << std::endl;
    //
    break;
  case CANDIDATE_POPED:
    hull.CreateHull(state.candiates, state.pointThatCausedPop);
    std::cout << "Candidate poped." << std::endl;

    break;

  case FINISHED:
    std::cout << "Algorithm finished." << std::endl;
    hull.CreateHull(state.hull.points);
    // when finished and this is set to zero the algo could start all over
    // again -> maybe wanted behaviour
    g_stepCounter = 0;
    return true;
    break;
  default:
    break;
  }
  return false;
}
void Animate() { animate = true; }
void SetupMenu(ResourceManager &resMan) {
  Button *increaseField = new Button(resMan, "+500", sf::Vector2f(195.f, 140.f),
                                     sf::Vector2f(100.f, 50.f));
  increaseField->setTriggerFunction([]() { UpdateFieldSize(500); });
  g_buttons.push_back(increaseField);

  Button *decreaseField = new Button(resMan, "-500", sf::Vector2f(305, 140.f),
                                     sf::Vector2f(100.f, 50.f));
  decreaseField->setTriggerFunction([]() { UpdateFieldSize(-500); });
  g_buttons.push_back(decreaseField);

  Button *stepHull = new Button(resMan, "Step", sf::Vector2f(180, 380));
  stepHull->setTriggerFunction([]() { TakeStep(); });
  g_buttons.push_back(stepHull);

  Button *calcHull = new Button(resMan, "Calc Hull", sf::Vector2f(180, 320));
  calcHull->setTriggerFunction([]() {
    PerformanceMonitor monitor;
    monitor.start();
    ConvexHull hull_res = findConvexHull(dots->m_point_set.get());
    float millis = monitor.stop();
    std::cout << dots->m_point_set->getSize()
              << " Points: convex hull calculated in "
              << PerformanceMonitor::millisToString(millis) << std::endl;

    hull.CreateHull(hull_res.points);
    // std::cout << hull.String();
  });
  g_buttons.push_back(calcHull);

  Button *genDots = new Button(resMan, "Gen Dots", sf::Vector2f(180, 260));
  genDots->setTriggerFunction([]() {
    for (int i = 0; i < g_numberOfPointsToGen; ++i) {
      dots->CreateDotWithLabelAndPushBack(
          Vec2f(random(0, static_cast<float>(g_fieldSize)),
                random(0, static_cast<float>(g_fieldSize))));
    }
    dots->CreateDotLabels();
    g_stepCounter = 0;
  });
  g_buttons.push_back(genDots);

  Button *clear = new Button(resMan, "Clear", sf::Vector2f(180, 560));
  clear->setTriggerFunction([]() {
    hull.Clear();
    dots->Clear();
  });
  g_buttons.push_back(clear);

  Button *toggleNumbers =
      new Button(resMan, "Toggle Nums", sf::Vector2f(180, 200));
  toggleNumbers->setTriggerFunction(
      []() { dots->m_render_dots = !dots->m_render_dots; });
  g_buttons.push_back(toggleNumbers);

  Button *step_back = new Button(resMan, "Step back", sf::Vector2f(180, 440));
  step_back->setTriggerFunction([]() {
    g_stepCounter -= 2;
    TakeStep();
  });
  g_buttons.push_back(step_back);

  Button *animate = new Button(resMan, "Animate", sf::Vector2f(180, 500));
  animate->setTriggerFunction([]() { Animate(); });
  g_buttons.push_back(animate);
}

//############################################################################
int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  /*PerformanceMonitor monitor;
  monitor.start();
  PointSet points;
  for(int i = 0; i < 10000000; ++i) {
          points.addPoint(*new Point(Vec2f(random(0, 10000), random(0,
  10000))));
  }
  ConvexHull hull_res = findConvexHull(&points);
  float millis = monitor.stop();
  std::cout << points.getSize() << " Points: convex hull calculated in " <<
  PerformanceMonitor::millisToString(millis) << std::endl;*/

  // fps timer
  float elapsedTime;
  float windowTitleCounter = 0;
  float animationCounter = 0;
  sf::Clock clock;

  // Globals
  ResourceManager resMan;
  g_resManager = &resMan;

  dots = new Dots(resMan);
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

    if (animate) {
      animationCounter += elapsedTime;
    }
    if (animationCounter > 0.25) {
      animationCounter = 0;
      bool isFinished = TakeStep();
      if (isFinished) {
        animate = false;
        std::cout << "Animation finished" << std::endl;
      }
    }
    // update
    if (input.isMouseOverSidebar) {
      for (auto &button : g_buttons) {
        button->Update(input);
      }
    }

    // draw the g_points ######################################################
    window.setView(g_gameView);

    hull.Draw(window);
    dots->Draw(window);

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