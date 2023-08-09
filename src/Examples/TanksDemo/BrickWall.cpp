#include "BrickWall.h"
#include "../../Renderer/Sprite.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../Renderer/ShowOutline.h"

BrickWall::BrickWall(std::shared_ptr<RenderEngine::Sprite> pSprite,
	glm::vec2& position,
	glm::vec2& size)
	: IGameObject(pSprite, "wall", position, size)
{
}
void BrickWall::render()
{
	m_pSprite->setSubTexture("BrickWall");
	m_pSprite->render(glm::vec2(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2), m_size / 2.f, 0, 0);
	m_pSprite->render(glm::vec2(m_position.x + m_size.x / 2, m_position.y), m_size / 2.f, 0, 0);
	m_pSprite->render(glm::vec2(m_position.x, m_position.y + m_size.y / 2), m_size / 2.f, 0, 0);
	m_pSprite->render(m_position, m_size / 2.f, 0, 0);
	if (getComponent<ShowOutline>("showOutline") != nullptr) getComponent<ShowOutline>("showOutline")->render();
}