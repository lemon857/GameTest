#pragma once

#include "EngineCore/IGameObject.h"
#include "EngineCore/Physics/Collider.h"
#include <memory>

namespace RenderEngine
{
	class SpriteRenderer;
}

class BrickWall : public IGameObject
{
public:
	BrickWall(std::shared_ptr<RenderEngine::SpriteRenderer> pSpriteRenderer, glm::vec2& position, glm::vec2& size);

	void render();
private:
	std::shared_ptr<Physics::Collider> m_pCollider;

};