#include "EngineCore/Input.h"

bool Input::keysPressed[] = {};
bool Input::buttonsPressed[] = {};

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

bool Input::isMouseButtonPressed(MouseButton key)
{
	return buttonsPressed[static_cast<size_t>(key)];
}

void Input::pressMouseButton(MouseButton key)
{
	buttonsPressed[static_cast<size_t>(key)] = true;
}

void Input::releaseMouseButton(MouseButton key)
{
	buttonsPressed[static_cast<size_t>(key)] = false;
}
