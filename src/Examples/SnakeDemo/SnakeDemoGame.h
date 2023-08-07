#pragma once

#include <vector>

#include "../../Engine/IGame.h"
#include "../../Renderer/Line.h"
#include "../../System/Timer.h"
#include "../../Physics/PhysicsEngine.h"
#include "SnakePart.h"

class SnakeDemoGame : public IGame
{
	enum EGameStatus
	{
		Active,
		Loose
	};
public:
	SnakeDemoGame(const glm::ivec2& windowSize, const glm::ivec2& cellSize);

	void render() const override;
	void update(const double delta) override;
	bool init() override;

	static void onTick();

private:
	void checkDead();
	void checkApple();

	glm::ivec2 m_cellSize;
	glm::ivec2 m_cellPlaceSize;
	glm::ivec2 m_posApple;

	Physics::EDirection m_eDir;
	EGameStatus m_status;

	bool m_flag = false;

	std::shared_ptr<RenderEngine::Line> m_pLine;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
	std::shared_ptr<RenderEngine::Sprite> m_pSpriteApple;
	std::shared_ptr<RenderEngine::Sprite> m_pSpriteEnd1;
	std::shared_ptr<RenderEngine::Sprite> m_pSpriteEnd2;
	std::shared_ptr<Timer> m_pTimer;

	std::vector<std::shared_ptr<SnakePart>> m_snakeParts;
};