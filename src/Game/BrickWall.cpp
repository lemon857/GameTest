#include "BrickWall.h"
#include "../Renderer/Sprite.h"
#include "../Physics/PhysicsEngine.h"

BrickWall::BrickWall(std::shared_ptr<RenderEngine::Sprite> pSprite,
	glm::vec2& position,
	glm::vec2& size)
	: IGameObject(pSprite, "wall", position, size)
	, m_pCollider(std::make_shared<Physics::Collider>(*this))
{
	Physics::PhysicsEngine::addCollider(m_pCollider);
}
void BrickWall::render() const
{
	/*m_pSprite->render(glm::vec2(m_position.x + m_size.x / 4, m_position.y + m_size.y / 4), glm::vec2(m_size.x / 4, m_size.y / 4), 0, 0);
	m_pSprite->render(glm::vec2(m_position.x + m_size.x / 4, m_position.y), glm::vec2(m_size.x / 4, m_size.y / 4), 0, 0);
	m_pSprite->render(glm::vec2(m_position.x, m_position.y + m_size.y / 4), glm::vec2(m_size.x / 4, m_size.y / 4), 0, 0);
	m_pSprite->render(m_position, glm::vec2(m_size.x / 4, m_size.y / 4), 0, 0);*/
	m_pSprite->setSubTexture("BrickWall");
	m_pSprite->render(m_position, m_size, 0, 0);
}