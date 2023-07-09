#pragma once

#include <memory>
#include <glm/vec2.hpp>

namespace Renderer
{
	class AnimatedSprite;
}

class Tank
{
public:
	enum class EOrentation
	{
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, glm::vec2& position);

	void render() const;
	void setOrentation(const EOrentation orentation);
	void move(const bool move);
	void update(const uint64_t delta);

private:
	EOrentation m_eOrentation;
	std::shared_ptr<Renderer::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 m_position;
	glm::vec2 m_moveOffset;
};