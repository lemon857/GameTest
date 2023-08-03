#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "../Engine/IGameObjec.h"
#include "../Engine/IComponent.h"

namespace RenderEngine
{
	class Sprite;
	class Animator;
}

class CharacterController;

class Tank : public IGameObject
{
public:
	enum class EOrentation
	{
		Top,
		Bottom,
		Left,    
		Right
	};

	Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size);

	void render() const;
	void setOrentation(const EOrentation orentation);
	void move(const bool move);
	void update(const double delta) override;

private:
	void startAnimation();
	EOrentation m_eOrentation;
	std::shared_ptr<RenderEngine::Animator> m_pAnimator;
	std::shared_ptr<CharacterController> m_pCharacterController;
}; 