#pragma once
#include "Marker.h"
#include "InputState.h"

class InputHandler {
	sf::RenderWindow* window;
	InputState inputState;

	Marker cursorIndicator;
	void pollEvents();
	void handleEvent(sf::Event& event);
	void HandleMouseEvent(sf::Event& event);
	void HandleKeyboardEvent(sf::Event& event);
public:
	InputHandler(sf::RenderWindow& window);
	void update(float elapsedTime);

	const InputState* getInputState() const;

	const sf::Drawable* getDrawable() const;
	~InputHandler();
};

