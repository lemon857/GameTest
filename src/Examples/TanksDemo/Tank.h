#pragma once

#include <memory>
#include <string>
#include <functional>
#include <glm/vec2.hpp>
#include "EngineCore/IGameObject.h"
#include "EngineCore/IComponent.h"
#include "EngineCore/Physics/PhysicsEngine.h"	
#include "Bullet.h"

namespace RenderEngine
{
	class SpriteRenderer;
	class Animator;
}

namespace Physics
{
	class Collider;
}

class MoveController;

class Tank : public IGameObject
{
public:

	Tank(std::shared_ptr<RenderEngine::SpriteRenderer> pSpriteRenderer, std::shared_ptr<RenderEngine::SpriteRenderer> pBulletSpriteRenderer, double velocity, double weight, glm::vec2& position, glm::vec2& size);

	void render();
	void setOrentation(const Physics::EDirection orentation);
	void move(const bool move);
	void update(const double delta) override;
	void fire();
private:
	void startAnimation();
	Physics::EDirection m_eOrentation;
	std::shared_ptr<Bullet> m_bullet;
}; 