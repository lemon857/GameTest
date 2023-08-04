#pragma once

#include <array>
#include<glm/vec2.hpp>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Renderer/Point.h"
#include "../Renderer/Line.h"
class Tank;
class BrickWall;

class Game 
{
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key, const int action);
	bool init();
private:
	std::array<bool, 349> m_keys;

	enum class EGameState {
		Active,
		Pause
	};

	EGameState m_eCurrentGameState;

	glm::ivec2 m_WindowSize;

	std::shared_ptr<RenderEngine::Point> m_point;
	std::shared_ptr<RenderEngine::Line> m_line;
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<BrickWall> m_pBrickWall;
};
