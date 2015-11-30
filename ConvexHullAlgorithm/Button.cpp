#include "Button.h"
#include "ResourceManager.h"
#include <iostream>
#include <string.h>

#include <SFML/Graphics/Font.hpp>

Button::Button(ResourceManager &resMan, std::string buttontxt,
               sf::Vector2f position)
    : m_triggerFunction(nullptr) {

  m_body.setSize(sf::Vector2f(350.f, 50.f));
  auto btnSize = m_body.getGlobalBounds();
  m_body.setOrigin(btnSize.width / 2.f, btnSize.height / 2.f);
  m_body.setPosition(position);

  m_text.setFont(resMan.GetFont(ResourceManager::DEFAULT));
  m_text.setCharacterSize(48);
  m_text.setString(buttontxt);
  m_text.setColor(sf::Color::White);

  auto textSize = m_text.getGlobalBounds();
  Vec2f textSizeHalf(textSize.width / 2.f, (textSize.height+3 ));
  m_text.setOrigin(textSizeHalf.x, textSizeHalf.y);
  m_text.setPosition(position.x, position.y + 8 );
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
    m_body.setFillColor(sf::Color(193, 133, 133, 133));

  } else {
    m_body.setFillColor(sf::Color(193, 255, 133, 133));
  }
}

void Button::setTriggerFunction(function triggerFunction) {
  m_triggerFunction = triggerFunction;
}
