#pragma once

#include "EngineCore/Keys.h"

class Input
{
public:
	Input() = delete;
	~Input() = delete;

	static bool isKeyPressed(KeyCode key);

	static void pressKey(KeyCode key);
	static void releaseKey(KeyCode key);

private:
	static bool keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)];
};