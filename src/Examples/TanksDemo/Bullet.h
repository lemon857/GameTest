#pragma once

#include <memory>

#include "../../Engine/IGameObjec.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../System/TemplateTimer.h"

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