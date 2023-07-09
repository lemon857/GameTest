#include "Tank.h"
#include "../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, glm::vec2& position)
	: m_eOrentation(EOrentation::Top)
	, m_pSprite(std::move(pSprite))
	, m_velocity(velocity)
	, m_position(position)
	, m_move(false)
	, m_moveOffset(glm::vec2(0.f, 1.f))
{
	m_pSprite->setPosition(position);
}
void Tank::render() const
{
	m_pSprite->render();
}
void Tank::setOrentation(const EOrentation orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	case Tank::EOrentation::Top:
		m_pSprite->setState("TopState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Tank::EOrentation::Bottom:
		m_pSprite->setState("BottomState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Tank::EOrentation::Left:
		m_pSprite->setState("LeftState");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Tank::EOrentation::Right:
		m_pSprite->setState("RightState");
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		m_pSprite->setState("TopState");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	}
}
void Tank::move(const bool move)
{
	m_move = move;
}
void Tank::update(const uint64_t delta)
{
	if (m_move) 
	{
		m_position += delta * m_velocity * m_moveOffset;
		m_pSprite->setPosition(m_position);
		m_pSprite->update(delta);
	}
}