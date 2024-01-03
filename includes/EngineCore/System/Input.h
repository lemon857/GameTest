#pragma once

#include "EngineCore/System/Keys.h"

#include <string>

class Input
{
public:
	Input() = delete;
	~Input() = delete;

	static std::string getKeyString(KeyCode key);
	static bool isKeyPressed(KeyCode key);

	static void pressKey(KeyCode key);
	static void releaseKey(KeyCode key);

	static bool isMouseButtonPressed(MouseButton key);

	static void pressMouseButton(MouseButton key);
	static void releaseMouseButton(MouseButton key);

	static KeyCode getLastKeyPressed();
private:
	static bool keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	static bool buttonsPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST)];
	static KeyCode lastKeyPressed;
};