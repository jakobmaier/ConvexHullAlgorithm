#include "InputHandler.h"


InputHandler::InputHandler(sf::RenderWindow& window)
: window(&window)
, inputState()
, cursorIndicator({ 0, 0 }, 5, 0) {
	cursorIndicator.setAppearance(2, 2, 16, sf::Color::Yellow);
}

void InputHandler::pollEvents() {
	inputState.leftMouseButtonClick = false;
	inputState.rightMouseButtonClick = false;

	sf::Event event;
	while(window->pollEvent(event)) {
		handleEvent(event);
	}
}



void InputHandler::handleEvent(sf::Event &event) {
	switch(event.type) {
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			HandleMouseEvent(event);
			break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			HandleKeyboardEvent(event);
			break;
		case sf::Event::Closed:
			window->close();
			break;
		default: break;
	}

}

void InputHandler::HandleMouseEvent(sf::Event& event) {
	switch(event.mouseButton.button) {
		case  sf::Mouse::Left:
			if(event.type == sf::Event::MouseButtonPressed) {
				inputState.leftMouseButtonPressed = true;
				inputState.leftMouseButtonClick = true;
			} else {
				inputState.leftMouseButtonPressed = false;
			}
			break;
		case  sf::Mouse::Right:
			if(event.type == sf::Event::MouseButtonPressed) {
				inputState.rightMouseButtonPressed = true;
				inputState.rightMouseButtonClick = true;
			} else {
				inputState.rightMouseButtonPressed = false;
			}
			break;
		default: break;
	}
}

void InputHandler::HandleKeyboardEvent(sf::Event& event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		window->close();
	}
}


void InputHandler::update(float elapsedTime) {
	(void)elapsedTime;

	pollEvents();

	sf::Vector2i mousePositionScreenCoords = sf::Mouse::getPosition(*window);
	inputState.mousePosition = window->mapPixelToCoords(mousePositionScreenCoords);
	cursorIndicator.setPosition(inputState.mousePosition);
}

const InputState* InputHandler::getInputState() const {
	return &inputState;
}

const sf::Drawable* InputHandler::getDrawable() const {
	return cursorIndicator.getDrawable();
}

InputHandler::~InputHandler() {
}
