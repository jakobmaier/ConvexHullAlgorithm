#pragma once
#include "Point.h"
#include "PointSet.h"
#include "ResourceManager.h"
#include "basics.h"
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <vector>

namespace sf {
class CircleShape;
class RenderWindow;
}

class Dots {
public:
  explicit Dots(ResourceManager &resMan);
  ~Dots();

  void Draw(sf::RenderWindow &window);
  void Clear();
  void ClearLabels();

  //void Clear();

  void CreateDotLabels();
  void CreateDotLabelAndPushBack(ResourceManager::Fonts font,
                                 std::string number, Vec2f pos);
  std::shared_ptr<sf::Text> CreateLabelForDot(ResourceManager::Fonts font,
                                              std::string number, Vec2f pos);

  void CreateDotWithLabelAndPushBack(Vec2f position);
  std::shared_ptr<sf::CircleShape> CreateDot(Vec2f position);

  std::vector<std::shared_ptr<sf::CircleShape>> m_dots;
  std::vector<std::shared_ptr<sf::Text>> m_dot_label;
  ResourceManager *resMan;
  bool m_render_dots;

  std::shared_ptr<sf::CircleShape> m_reference_point;
  std::shared_ptr<PointSet> m_point_set;
};
