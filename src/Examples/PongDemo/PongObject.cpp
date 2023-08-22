#include "PongObject.h"

//#include "EngineCore/Renderer/ShowOutline.h"

PongObject::PongObject(std::shared_ptr<RenderEngine::SpriteRenderer> SpriteRenderer, const std::string name, const glm::vec2& position, const glm::vec2& size, const double velocity)
	: IGameObject(name)
	, m_eOrentation(Physics::EDirection::Left)
{

}
void PongObject::render()
{
	//getComponent<ShowOutline>("showOutline")->render();
}
void PongObject::setOrentation(const Physics::EDirection orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	/*case Physics::EDirection::Up:
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Physics::EDirection::Down:
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;*/
	}
}
void PongObject::move(const bool move)
{
	//m_move = move;
}