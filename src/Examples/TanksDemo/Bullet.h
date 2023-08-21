#pragma once

#include <memory>

#include "EngineCore/IGameObject.h"
#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/System/TemplateTimer.h"

namespace RenderEngine
{
	class Sprite;
}

enum EBulletState
{
	Active,
	Passive
};

class Bullet : public IGameObject
{
public:
	Bullet(
		std::shared_ptr<RenderEngine::Sprite> sprite,
		const std::string name,
		const glm::vec2& position,
		const glm::vec2& size,
		double liveTime,
		double velocity);

	void update(double delta) override;
	void render();

	void fire(const glm::vec2& position, Physics::EDirection dir);
	void dead();

	bool isDead();
private:
	TemplateTimer<Bullet*> m_timerLive; 
	EBulletState m_state;
	double m_liveTime;
};