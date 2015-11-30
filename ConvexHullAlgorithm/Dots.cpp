#include "Config.h"
#include "Dots.h"

Dots::Dots(ResourceManager &resMan) : resMan(&resMan), m_render_dots(true) {
  std::shared_ptr<PointSet> point_set(new PointSet());
  m_point_set = point_set;
}

Dots::~Dots() {}

void Dots::Draw(sf::RenderWindow &window) {
  for (auto &dot : m_dots) {
    window.draw(*dot);
  }
  if (m_render_dots) {
    for (auto &label : m_dot_label) {
      window.draw(*label);
    }
  }
}

void Dots::Clear() {
  m_dots.clear();
  ClearLabels();
  m_point_set->clear();
  m_reference_point.reset();
}

void Dots::ClearLabels() { m_dot_label.clear(); }

void Dots::CreateDotLabels() {
  int counter = 0;
  for (const auto *point : m_point_set->points) {
    CreateDotLabelAndPushBack(ResourceManager::HACK, std::to_string(counter),
                              point->pos());
    ++counter;
  }
}

void Dots::CreateDotLabelAndPushBack(ResourceManager::Fonts font,
                                     std::string number, Vec2f pos) {
  auto label = CreateLabelForDot(font, number, pos);
  m_dot_label.push_back(label);
}

std::shared_ptr<sf::Text> Dots::CreateLabelForDot(ResourceManager::Fonts font,
                                                  std::string number,
                                                  Vec2f pos) {
  std::shared_ptr<sf::Text> label(new sf::Text());
  label->setFont(resMan->GetFont(font));
  label->setCharacterSize(32);
  label->setPosition(pos.x, pos.y - 5);
  label->setColor(DOT_NUMBER_TEXT);
  label->setString(number);
  return label;
}

// creates the label too
void Dots::CreateDotWithLabelAndPushBack(Vec2f position) {
  auto dot = CreateDot(position);
  m_dots.push_back(dot);

  if (m_point_set->addPoint(*new Point(position))) {
    if (m_reference_point != nullptr) {
      m_reference_point->setFillColor(DOT_COLOR);
    }
    m_reference_point = m_dots.back();
    m_reference_point->setFillColor(DOT_REFERENCE_COLOR);
  }
  CreateDotLabelAndPushBack(ResourceManager::HACK, std::to_string(m_dots.size() - 1),
                    position);
}

std::shared_ptr<sf::CircleShape> Dots::CreateDot(Vec2f position) {
  std::shared_ptr<sf::CircleShape> dot(new sf::CircleShape());
  dot->setFillColor(DOT_COLOR);
  dot->setRadius(5.f);
  dot->setOrigin(5.f, 5.f);
  dot->setPosition(position);
  return dot;
}