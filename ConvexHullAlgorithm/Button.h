#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "ResourceManager.h"


typedef void(*function)(void);


class Button
{
public:
	Button(ResourceManager& resMan, std::string buttontxt, sf::Vector2f position);
	~Button();

	void Draw(sf::RenderWindow& window) const;
	void Update(Input input);
	void setTriggerFunction(function triggerFunction);
private:
	sf::RectangleShape m_body;
	sf::Text m_text;

	function m_triggerFunction;
};

