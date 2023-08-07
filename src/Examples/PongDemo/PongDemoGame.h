#pragma once

#include "../../Engine/IGame.h"
#include "../../Renderer/Point.h"

#include "PongObject.h"
#include "Ball.h"

class PongDemoGame : public IGame
{
	enum EGameStatus
	{
		Active,
		Loose
	};
public:
	PongDemoGame(glm::ivec2 windowSize);

	void render() const override;
	void update(const double delta) override;
	bool init() override;

private:
	EGameStatus m_status;

	std::shared_ptr<PongObject> m_downWall;
	std::shared_ptr<PongObject> m_upWall;

	std::shared_ptr<PongObject> m_plate1;

	std::shared_ptr<Ball> m_ball;
};