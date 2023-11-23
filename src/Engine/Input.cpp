#include "EngineCore/System/Input.h"

bool Input::keysPressed[] = {};
bool Input::buttonsPressed[] = {};
KeyCode Input::lastKeyPressed = KeyCode::KEY_UNKNOWN;

bool Input::isKeyPressed(KeyCode key)
{
	return keysPressed[static_cast<size_t>(key)];
}

void Input::pressKey(KeyCode key)
{
	keysPressed[static_cast<size_t>(key)] = true;
	lastKeyPressed = key;
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

KeyCode Input::getLastKeyPressed()
{
	return lastKeyPressed;
}
