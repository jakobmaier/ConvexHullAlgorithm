#include "Hull.h"
#include "LineShape.h"
#include <iostream>
#include <memory>

Hull::Hull() {}

Hull::~Hull() {}

// sfml draw
void Hull::Draw(sf::RenderWindow &window) {
  for (auto &line : m_hull_lines) {
    window.draw(*line.get());
  }
}

// create hull from stack, this function is also capable of showing deleting
// lines and soon to be
// created line too
void Hull::CreateHull(std::stack<const Point *> &points,
                      const Point *point_that_caused_pop) {
  if (points.size() < 2) {
    std::cerr << "Not enough dots to create a hull. Ignoring..." << std::endl;
    return;
  }
  m_hull_lines.clear();

  const Point *elem;
  const Point *elemNext;

  elem = points.top();
  points.pop();

  // if there is a point present a line has to be deleted
  if (point_that_caused_pop != nullptr) {
    PushBackNewLine(point_that_caused_pop->pos(), elem->pos(),
                    LINE_COLOR_DELETE);
    PushBackNewLine(point_that_caused_pop->pos(), points.top()->pos(),
                    LINE_COLOR_NEW);
  }
  do {
    elemNext = points.top();
    points.pop();
    if (point_that_caused_pop != nullptr) { // "delete" the first line
      PushBackNewLine(elem->pos(), elemNext->pos(), LINE_COLOR_DELETE);
      point_that_caused_pop = nullptr;
    } else {
      PushBackNewLine(elem->pos(), elemNext->pos());
    }
    elem = elemNext;
  } while (!points.empty());
}

// go through all points and make line pairs that are pushed to the hull vector
// this function relies on the fact that the zeroth point is twice in the vector
// to
// close the circle
void Hull::CreateHull(std::vector<const Point *> &points) {
  if (points.size() < 2) {
    std::cerr << "Not enough dots to create a hull. Ignoring..." << std::endl;
    return;
  }
  m_hull_lines.clear();
  auto it = points.begin();
  auto itNext = points.begin() + 1;

  while (itNext != points.end()) {
    PushBackNewLine((*it)->pos(), (*itNext)->pos());
    ++it;
    ++itNext;
  }
}

void Hull::PushBackNewLine(Vec2f pos1, Vec2f pos2, sf::Color color) {
  auto line = CreateLine(pos1, pos2, color);
  m_hull_lines.push_back(line);
}

// Create a line between two points, with a destinct color
std::shared_ptr<LineShape> Hull::CreateLine(Vec2f pos1, Vec2f pos2,
                                            sf::Color color) const {
  std::shared_ptr<LineShape> line(new LineShape(pos1, pos2));
  line->setThickness(10.f);
  line->setFillColor(color);
  return line;
}

void Hull::Clear() { m_hull_lines.clear(); }