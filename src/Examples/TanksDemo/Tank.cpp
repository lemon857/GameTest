#include "Tank.h"
//#include "EngineCore/Renderer/SpriteRenderer.h"
#include "EngineCore/Renderer/Animator.h"
//#include "EngineCore/Renderer/ShowOutline.h"
#include "EngineCore/Renderer/Animation.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Physics/MoveController.h"
#include "EngineCore/Physics/Collider.h"
#include "EngineCore/Physics/PhysicsEngine.h"

Tank::Tank(std::shared_ptr<RenderEngine::SpriteRenderer> pSpriteRenderer,
	std::shared_ptr<RenderEngine::SpriteRenderer> pBulletSpriteRenderer,
	double velocity, double weight,
	glm::vec2& position, glm::vec2& size)
	: m_eOrentation(Physics::EDirection::Up)
	, m_bullet(std::make_shared<Bullet>(pBulletSpriteRenderer, "bullet", position + size / 2.f, size / 2.f, 2000, 0.5))
	, IGameObject("tank")
{
	std::shared_ptr<Physics::Collider> bulCol = std::make_shared<Physics::Collider>();
	/*bulCol->setOnCollisionCallback(
	[](IGameObject& targetObj, IGameObject& obj, Physics::EDirection dir)
	{
		if (obj.getName() == "wall")
		{
			switch (dir)
			{
			case Physics::Up:
				if (targetObj.getMoveOffset().y > 0) targetObj.setMoveOffset(glm::vec2(targetObj.getMoveOffset().x, 0.f));
				break;
			case Physics::Down:
				if (targetObj.getMoveOffset().y < 0) targetObj.setMoveOffset(glm::vec2(targetObj.getMoveOffset().x, 0.f));
				break;
			case Physics::Left:
				if (targetObj.getMoveOffset().x < 0) targetObj.setMoveOffset(glm::vec2(0.f, targetObj.getMoveOffset().y));
				break;
			case Physics::Right:
				if (targetObj.getMoveOffset().x > 0) targetObj.setMoveOffset(glm::vec2(0.f, targetObj.getMoveOffset().y));
				break;
			}
		}
	}));
	m_bullet->addComponent("moveController", std::make_shared<MoveController>(*m_bullet));
	m_bullet->addComponent("collider", bulCol);*/
}
void Tank::render()
{
	//m_pSpriteRenderer->render(m_position, m_size, 0, 1);
	//getComponent<ShowOutline>("showOutline")->render();
	if (m_bullet != nullptr) m_bullet->render();
}
void Tank::setOrentation(const Physics::EDirection orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	case Physics::EDirection::Up:
		//m_pSpriteRenderer->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		//m_moveOffset.x = 0.f;
		//m_moveOffset.y = 1.f;
		break;
	case Physics::EDirection::Down:
		//m_pSpriteRenderer->setState("BottomState");
		//m_pAnimator->startAnimation("BottomMove");
		//m_moveOffset.x = 0.f;
		//m_moveOffset.y = -1.f;
		break;
	case Physics::EDirection::Left:
		//m_pSpriteRenderer->setState("LeftState");
		//m_pAnimator->startAnimation("LeftMove");
		//m_moveOffset.x = -1.f;
		//m_moveOffset.y = 0.f;
		break;
	case Physics::EDirection::Right:
		//m_pSpriteRenderer->setState("RightState");
		//m_pAnimator->startAnimation("RightMove");
		//m_moveOffset.x = 1.f;
		//m_moveOffset.y = 0.f;
		break;
	default:
		//m_pSpriteRenderer->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		//m_moveOffset.x = 0.f;
		//m_moveOffset.y = 1.f;
		break;
	}
	startAnimation();
}
void Tank::startAnimation()
{
	//RenderEngine::Animator* m_pAnimator = getComponent<RenderEngine::Animator>("animator");
	switch (m_eOrentation)
	{
	case Physics::EDirection::Up:
		//m_pAnimator->startAnimation("TopMove");
		break;
	case Physics::EDirection::Down:
		//m_pAnimator->startAnimation("BottomMove");
		break;
	case Physics::EDirection::Left:
		//m_pAnimator->startAnimation("LeftMove");
		break;
	case Physics::EDirection::Right:
		//m_pAnimator->startAnimation("RightMove");
		break;
	default:
		//m_pAnimator->startAnimation("TopMove");
		break;
	}
}
void Tank::move(const bool move)
{
	//RenderEngine::Animator* m_pAnimator = getComponent<RenderEngine::Animator>("animator");
	//m_move = move;
	//if (move == false) m_pAnimator->stopAnimations();
	//else startAnimation();
}
void Tank::update(const double delta) 
{
	if (m_bullet != nullptr) m_bullet->update(delta);
	updateComponents(delta);
}
void Tank::fire()
{
	//if (m_bullet != nullptr && m_bullet->isDead()) m_bullet->fire(m_position + m_size / 4.f, m_eOrentation);
}
