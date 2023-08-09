#include "Bullet.h"

#include "EngineCore/Renderer/Sprite.h"

void deadCallback(Bullet* bullet)
{
	bullet->dead();
}

Bullet::Bullet(
	std::shared_ptr<RenderEngine::Sprite> sprite,
	const std::string name,
	const glm::vec2& position,
	const glm::vec2& size,
	double liveTime,
	double velocity)
	: IGameObject(sprite, name, position, size, glm::vec2(0), velocity)
	, m_timerLive(this)
	, m_state(Passive)
	, m_liveTime(liveTime)
{
	m_move = true;
}

void Bullet::update(double delta)
{
	if (m_state == Passive) return;
	m_timerLive.update(delta);
	updateComponents(delta);
}

void Bullet::render()
{
	if (m_state == Passive) return;
	m_pSprite->render(m_position, m_size, 0, 0);
}

void Bullet::fire(const glm::vec2& position, Physics::EDirection dir)
{
	m_timerLive.setTemplateCallback(deadCallback);
	m_position = position;
	switch (dir)
	{
	case Physics::Up:
		m_pSprite->setSubTexture("bulletUp");
		m_moveOffset = glm::vec2(0.f, 1.f);
		break;
	case Physics::Down:
		m_pSprite->setSubTexture("bulletDown");
		m_moveOffset = glm::vec2(0.f, -1.f);
		break;
	case Physics::Left:
		m_pSprite->setSubTexture("bulletLeft");
		m_moveOffset = glm::vec2(-1.f, 0.f);
		break;
	case Physics::Right:
		m_pSprite->setSubTexture("bulletRight");
		m_moveOffset = glm::vec2(1.f, 0.f);
		break;
	}
	m_state = Active;
	m_timerLive.start(m_liveTime);
}

void Bullet::dead()
{
	m_moveOffset = glm::vec2(0);
	m_state = Passive;
}

bool Bullet::isDead()
{
	if (m_state == Passive) return true;
	return false;
}
