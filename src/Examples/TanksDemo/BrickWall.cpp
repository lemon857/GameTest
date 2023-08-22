#include "BrickWall.h"
//#include "EngineCore/Renderer/SpriteRenderer.h"
#include "EngineCore/Physics/PhysicsEngine.h"
//#include "EngineCore/Renderer/ShowOutline.h"

BrickWall::BrickWall(std::shared_ptr<RenderEngine::SpriteRenderer> pSpriteRenderer,
	glm::vec2& position,
	glm::vec2& size)
	: IGameObject("wall")
{
}
void BrickWall::render()
{
	//m_pSpriteRenderer->setSubTexture("BrickWall");
	//m_pSpriteRenderer->render(glm::vec2(m_position.x + m_size.x / 2, m_position.y + m_size.y / 2), m_size / 2.f, 0, 0);
	//m_pSpriteRenderer->render(glm::vec2(m_position.x + m_size.x / 2, m_position.y), m_size / 2.f, 0, 0);
	//m_pSpriteRenderer->render(glm::vec2(m_position.x, m_position.y + m_size.y / 2), m_size / 2.f, 0, 0);
	//m_pSpriteRenderer->render(m_position, m_size / 2.f, 0, 0);
	//if (getComponent<ShowOutline>("showOutline") != nullptr) getComponent<ShowOutline>("showOutline")->render();
}