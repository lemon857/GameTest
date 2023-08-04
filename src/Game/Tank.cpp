#include "Tank.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Animator.h"
#include "../Renderer/Animation.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/CharacterController.h"
#include "../Physics/Collider.h"
#include "../Physics/PhysicsEngine.h"	
#include <iostream>

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size)
	: m_eOrentation(Physics::EDirection::Up)
	, IGameObject(std::move(pSprite), "tank", position, size, glm::vec2(0.f, 1.f), velocity, weight)
{	

}
void Tank::render() const
{
	m_pSprite->render(m_position, m_size, 0, 1);
}
void Tank::setOrentation(const Physics::EDirection orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	case Physics::EDirection::Up:
		//m_pSprite->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Physics::EDirection::Down:
		//m_pSprite->setState("BottomState");
		//m_pAnimator->startAnimation("BottomMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Physics::EDirection::Left:
		//m_pSprite->setState("LeftState");
		//m_pAnimator->startAnimation("LeftMove");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Physics::EDirection::Right:
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
void Tank::startAnimation()
{
	std::shared_ptr<RenderEngine::Animator> m_pAnimator = getComponent<RenderEngine::Animator>("animator");
	switch (m_eOrentation)
	{
	case Physics::EDirection::Up:
		m_pAnimator->startAnimation("TopMove");
		break;
	case Physics::EDirection::Down:
		m_pAnimator->startAnimation("BottomMove");
		break;
	case Physics::EDirection::Left:
		m_pAnimator->startAnimation("LeftMove");
		break;
	case Physics::EDirection::Right:
		m_pAnimator->startAnimation("RightMove");
		break;
	default:
		m_pAnimator->startAnimation("TopMove");
		break;
	}
}
void Tank::move(const bool move)
{
	//std::shared_ptr<RenderEngine::Animator> m_pAnimator = getComponent<RenderEngine::Animator>("animator");
	m_move = move;
	//if (move == false) m_pAnimator->stopAnimations();
	//else startAnimation();
}
void Tank::update(const double delta) 
{
	updateComponents(delta);
}