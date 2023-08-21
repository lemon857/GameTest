#pragma once

#include <memory>
#include <glm/vec2.hpp>

#include "EngineCore/IGameObject.h"
#include "EngineCore/Physics/PhysicsEngine.h"

namespace RenderEngine
{
	class SpriteRenderer;
}

class PongObject : public IGameObject
{
public:
	PongObject(std::shared_ptr<RenderEngine::SpriteRenderer> SpriteRenderer,
		const std::string name,
		const glm::vec2& position,
		const glm::vec2& size,
		const double velocity = 1);

	virtual void render();

	void setOrentation(const Physics::EDirection orentation);

	void move(const bool move);
private:
	Physics::EDirection m_eOrentation;
};