#include "EngineCore/Input.h"

bool Input::keysPressed[] = {};

bool Input::isKeyPressed(KeyCode key)
{
	return keysPressed[static_cast<size_t>(key)];
}

void Input::pressKey(KeyCode key)
{
	keysPressed[static_cast<size_t>(key)] = true;
}

void Input::releaseKey(KeyCode key)
{
	keysPressed[static_cast<size_t>(key)] = false;
}
