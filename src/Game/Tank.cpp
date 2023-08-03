#include "Tank.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Animator.h"
#include "../Renderer/Animation.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/CharacterController.h"

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size)
	: m_eOrentation(EOrentation::Top)
	, m_pAnimator(std::make_shared<RenderEngine::Animator>(*this))
	, m_pCharacterController(std::make_shared<CharacterController>(*this))
	, IGameObject(std::move(pSprite), position, size, glm::vec2(0.f, 1.f), velocity, weight)
{
	std::vector<std::string> subTextureNames = { "YellowUp11",	"YellowUp12" };
	std::vector<double> durations = { 100,	100 };
	m_pAnimator->addAnimation("TopMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
		RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

	subTextureNames = { "YellowDown11",	"YellowDown12" };
	m_pAnimator->addAnimation("BottomMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
		RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

	subTextureNames = { "YellowLeft11",	"YellowLeft12" };
	m_pAnimator->addAnimation("LeftMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
		RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

	subTextureNames = { "YellowRight11",	"YellowRight12" };
	m_pAnimator->addAnimation("RightMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
		RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));
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
void Tank::startAnimation()
{
	switch (m_eOrentation)
	{
	case Tank::EOrentation::Top:
		m_pAnimator->startAnimation("TopMove");
		break;
	case Tank::EOrentation::Bottom:
		m_pAnimator->startAnimation("BottomMove");
		break;
	case Tank::EOrentation::Left:
		m_pAnimator->startAnimation("LeftMove");
		break;
	case Tank::EOrentation::Right:
		m_pAnimator->startAnimation("RightMove");
		break;
	default:
		m_pAnimator->startAnimation("TopMove");
		break;
	}
}
void Tank::move(const bool move)
{
	m_move = move;
	if (move == false) m_pAnimator->stopAnimations();
	else startAnimation();
}
void Tank::update(const double delta) 
{
	m_pAnimator->update(delta);
	m_pCharacterController->update(delta);
}