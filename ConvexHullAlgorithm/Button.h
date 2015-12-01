#pragma once
#include "Hull.h"
#include "Input.h"
#include "Menu.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
  Button(ResourceManager &resMan, std::string buttontxt, sf::Vector2f position,
         sf::Vector2f size = sf::Vector2f(350.f, 50.f));
  ~Button();

  void Draw(sf::RenderWindow &window) const;
  void Update(Input input);
  void setTriggerFunction(std::function<void(void)> triggerFunction,
                          Menu &menu);
  void SetText(std::string text);
  void SetNumberText(double number);
  void SetActiveColor(sf::Color color);
  void SetDefaultColor(sf::Color color);

private:
  sf::RectangleShape m_body;
  sf::Text m_text;

  sf::Color m_default_color;
  sf::Color m_default_active_color;

  std::function<void(void)> m_triggerFunction;
  Menu *m_menu;
};
