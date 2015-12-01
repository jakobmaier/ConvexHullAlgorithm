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
#include "Menu.h"

// move into main scope

Hull hull;
Dots *dots;
Menu *menu;
//#####################

//std::vector<Button *> g_buttons;

sf::View g_gameView;
sf::View g_sideBarView;
//Button g_fieldSizeIndicator;
int g_fieldSize = 500;
int g_numberOfPointsToGen = NR_OF_POINTS_TO_GENERATE;

int g_stepCounter = 0;

//######################################################################

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

        std::cout << deltaMouse.x << std::endl;
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
  //g_fieldSizeIndicator.SetNumberText(g_fieldSize);
}

//void SetupMenu(ResourceManager &) {
  //Button *increaseField = new Button(resMan, "+500", sf::Vector2f(195.f, 140.f),
  //                                   sf::Vector2f(100.f, 50.f));
  //increaseField->setTriggerFunction([]() { UpdateFieldSize(500); });
  //g_buttons.push_back(increaseField);

  //Button *decreaseField = new Button(resMan, "-500", sf::Vector2f(305, 140.f),
  //                                   sf::Vector2f(100.f, 50.f));
  //decreaseField->setTriggerFunction([]() { UpdateFieldSize(-500); });
  //g_buttons.push_back(decreaseField);

  //Button *stepHull = new Button(resMan, "Step", sf::Vector2f(180, 200));

  //stepHull->setTriggerFunction([]() {

  //  std::cout << g_stepCounter << std::endl;

  //  HullState state =
  //      findConvexHullStep(dots->m_point_set.get(), g_stepCounter);
  //  g_stepCounter = state.step;
  //  switch (state.state) {
  //  case SORT_DONE: {
  //    std::cout << "Sort done." << std::endl;
  //    dots->ClearLabels();
  //    dots->CreateDotLabels();
  //    hull.Clear();
  //  } break;

  //  case CANDIDATE_ADDED:
  //    hull.CreateHull(state.candiates);
  //    std::cout << "Candidate(s) added." << std::endl;
  //    //
  //    break;
  //  case CANDIDATE_POPED:
  //    hull.CreateHull(state.candiates, state.pointThatCausedPop);
  //    std::cout << "Candidate poped." << std::endl;

  //    break;

  //  case FINISHED:
  //    std::cout << "Algorithm finished." << std::endl;
  //    // when finished and this is set to zero the algo could start all over
  //    // again -> maybe wanted behaviour
  //    g_stepCounter = 0;
  //    break;
  //  default:
  //    break;
  //  }
  //});
  //g_buttons.push_back(stepHull);

  //Button *calcHull = new Button(resMan, "Calc Hull", sf::Vector2f(180, 260));
  //calcHull->setTriggerFunction([]() {
  //  ConvexHull hull_res = findConvexHull(dots->m_point_set.get());
  //  hull.CreateHull(hull_res.points);
  //  // std::cout << hull.String();
  //});
  //g_buttons.push_back(calcHull);

  //Button *genDots = new Button(resMan, "Gen Dots", sf::Vector2f(180, 320));
  //genDots->setTriggerFunction([]() {
  //  for (int i = 0; i < g_numberOfPointsToGen; ++i) {
  //    dots->CreateDotWithLabelAndPushBack(
  //        Vec2f(random(0, static_cast<float>(g_fieldSize)),
  //              random(0, static_cast<float>(g_fieldSize))));
  //  }
  //  dots->CreateDotLabels();
  //  g_stepCounter = 0;
  //});
  //g_buttons.push_back(genDots);

  //Button *clear = new Button(resMan, "Clear", sf::Vector2f(180, 380));
  //clear->setTriggerFunction([]() {
  //  hull.Clear();
  //  dots->Clear();
  //});
  //g_buttons.push_back(clear);
//}

//############################################################################
int main() {

  srand(static_cast<unsigned int>(time(nullptr)));

  // fps timer
  float elapsedTime;
  float windowTitleCounter = 0;
  sf::Clock clock;

  // Globals
  ResourceManager resMan;

  dots = new Dots(resMan);
  menu = new Menu(resMan, hull, *dots);

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE,
                          sf::Style::Default, settings);
  g_sideBarView = CreateView(sf::Vector2u(WINDOW_X, WINDOW_Y), true);
  g_gameView = CreateView(sf::Vector2u(1600, 1200), false);





  //SetupMenu(resMan);

  UpdateFieldSize(0);

  sf::RectangleShape menuBackground(sf::Vector2f(WINDOW_X, WINDOW_Y));
  menuBackground.setFillColor(MENU_BACKGROUND_COLOR);

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

		menu->Update(input);

	} else
	{
		if (input.leftMouseClicked) {
			dots->CreateDotWithLabelAndPushBack(
				input.mousePos);
			g_stepCounter = 0;
		}
	}
    // draw the Dots and Hull #################################################
    window.setView(g_gameView);

    hull.Draw(window);
    dots->Draw(window);

    // Draw the menu here #####################################################
    window.setView(g_sideBarView);
	menu->Draw(window);

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