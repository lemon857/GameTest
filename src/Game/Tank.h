#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "../Engine/IGameObjec.h"
#include "../Engine/IComponent.h"
#include "../Physics/PhysicsEngine.h"	

namespace RenderEngine
{
	class Sprite;
	class Animator;
}

namespace Physics
{
	class Collider;
}

class CharacterController;

class Tank : public IGameObject
{
public:

	Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size);

	void render() const;
	void setOrentation(const Physics::EDirection orentation);
	void move(const bool move);
	void update(const double delta) override;

private:
	void startAnimation();
	Physics::EDirection m_eOrentation;
}; 