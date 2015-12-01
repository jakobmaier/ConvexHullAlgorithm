#include "Button.h"
#include "ResourceManager.h"
#include <iostream>
#include "Config.h"

#include <SFML/Graphics/Font.hpp>

Button::Button()
{
}

Button::Button(ResourceManager &resMan, std::string buttontxt,
	sf::Vector2f position, sf::Vector2f size)
    : m_default_color(BTN_DEFAULT_COLOR), m_default_active_color(BTN_DEFAULT_ACTIVE_COLOR), m_triggerFunction(nullptr) {

  m_body.setSize(size);
  auto btnSize = m_body.getGlobalBounds();
  m_body.setOrigin(btnSize.width / 2.f, btnSize.height / 2.f);
  m_body.setPosition(position);
  m_body.setOutlineThickness(3.f);
  m_body.setOutlineColor(sf::Color::Black);

  m_text.setFont(resMan.GetFont(ResourceManager::DEFAULT));
  m_text.setCharacterSize(48);
  m_text.setColor(sf::Color::White);
  m_text.setPosition(position.x, position.y+8);
  SetText(buttontxt);

  Update(Input());
}

Button::~Button() {}

void Button::Draw(sf::RenderWindow &window) const {
  window.draw(m_body);
  window.draw(m_text);
}

void Button::Update(Input input) {
  if (m_body.getGlobalBounds().contains(input.mousePos)) {
    if (input.leftMouseClicked) {
      auto str = m_text.getString();
      std::string test(str);
      if (m_triggerFunction != nullptr) {
        m_triggerFunction();
      }
      std::cout << test << " clicked." << std::endl;
    }
    m_body.setFillColor(m_default_active_color);

  } else {
    m_body.setFillColor(m_default_color);
  }
}

void Button::setTriggerFunction(function triggerFunction) {
  m_triggerFunction = triggerFunction;
}

void Button::SetText(std::string text)
{
	auto position = m_text.getPosition();
	m_text.setString(text);
	auto textSize = m_text.getGlobalBounds();
	Vec2f textSizeHalf(textSize.width / 2.f, (textSize.height + 3));
	m_text.setOrigin(textSizeHalf.x, textSizeHalf.y);
	m_text.setPosition(position.x, position.y);
}

void Button::SetNumberText(double number)
{
	if (number < 1000)
	{
		SetText(std::to_string((int)number));
	} else
	{
		SetText(std::to_string((int)number/1000) + " K");
	}
}

void Button::SetActiveColor(sf::Color color)
{
	m_default_active_color = color;
}

void Button::SetDefaultColor(sf::Color color)
{
	m_default_color = color;
}