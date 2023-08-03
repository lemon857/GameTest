#include "Tank.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Animator.h"
#include "../Resources/ResourceManager.h"

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size)
	: m_eOrentation(EOrentation::Top)
	, IGameObject(std::move(pSprite), position, size, glm::vec2(0.f, 1.f), velocity, weight)
{
	//m_pAnimator = ResourceManager::getAnimator("moveAnimator");
}
void Tank::render() const
{
	m_pSprite->render(m_position, m_size, 0, 1);
}
void Tank::setOrentation(const EOrentation orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	case Tank::EOrentation::Top:
		//m_pSprite->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Tank::EOrentation::Bottom:
		//m_pSprite->setState("BottomState");
		//m_pAnimator->startAnimation("BottomMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Tank::EOrentation::Left:
		//m_pSprite->setState("LeftState");
		//m_pAnimator->startAnimation("LeftMove");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Tank::EOrentation::Right:
		//m_pSprite->setState("RightState");
		//m_pAnimator->startAnimation("RightMove");
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		//m_pSprite->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	}
}
void Tank::move(const bool move)
{
	m_move = move;
}
void Tank::update(const double delta) 
{
	//m_pAnimator->update(delta);
}