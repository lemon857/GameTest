#pragma once

#include <array>

class Game 
{
public:
	Game();
	~Game();

	void render();
	void update(const uint64_t delta);
	void setKey(const int key, const int action);
	bool init();
private:
	std::array<bool, 349> m_keys;

	enum class EGameState {
		Active,
		Pause
	};

	EGameState m_eCurrentGameState;
};
