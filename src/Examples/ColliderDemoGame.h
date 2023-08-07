#pragma once

#include <array>
#include<glm/vec2.hpp>
#include "../Renderer/Point.h"
#include "../Renderer/Line.h"
#include "../Engine/IGame.h"

class Tank;
class BrickWall;

class ColliderDemoGame : public IGame
{
public:
	ColliderDemoGame(const glm::ivec2& windowSize);
	~ColliderDemoGame();

	void render() const override;
	void update(const double delta) override;
	bool init() override;
private:
	std::shared_ptr<RenderEngine::Point> m_point;
	std::shared_ptr<RenderEngine::Line> m_line;
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<BrickWall> m_pBrickWall;
	std::shared_ptr<BrickWall> m_pBrickWall2;
};
