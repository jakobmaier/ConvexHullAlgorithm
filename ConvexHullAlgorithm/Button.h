#pragma once
#include "Input.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

typedef void (*function)(void);

class Button {
public:
  Button();
  Button(ResourceManager &resMan, std::string buttontxt, sf::Vector2f position,
         sf::Vector2f size = sf::Vector2f(350.f, 50.f));
  ~Button();

  void Draw(sf::RenderWindow &window) const;
  void Update(Input input);
  void setTriggerFunction(function triggerFunction);
  void SetText(std::string text);
	void SetNumberText(double number);
	void SetActiveColor(sf::Color color);
  void SetDefaultColor(sf::Color color);

private:
  sf::RectangleShape m_body;
  sf::Text m_text;

  sf::Color m_default_color;
  sf::Color m_default_active_color;

  function m_triggerFunction;
};
