#pragma once
#include "basics.h"

struct InputState {
	Vec2f mousePosition;
	bool leftMouseButtonPressed;
	bool leftMouseButtonClick;
	
	bool rightMouseButtonPressed;
	bool rightMouseButtonClick;

	InputState();
};

