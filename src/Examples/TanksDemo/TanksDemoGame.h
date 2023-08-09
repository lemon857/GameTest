#pragma once

#include <array>
#include<glm/vec2.hpp>
#include "EngineCore/Renderer/Point.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/IGame.h"

class Tank;
class BrickWall;

class TanksDemoGame : public IGame
{
public:
	TanksDemoGame(const glm::ivec2& windowSize);
	~TanksDemoGame();

	void render() const override;
	void update(const double delta) override;
	bool init() override;
private:
	std::shared_ptr<RenderEngine::Line> m_line;
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<BrickWall> m_pBrickWall;
	std::shared_ptr<BrickWall> m_pBrickWall2;
};
