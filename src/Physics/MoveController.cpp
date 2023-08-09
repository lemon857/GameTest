#include "EngineCore/Physics/MoveController.h"

MoveController::MoveController(IGameObject& targetObj)
	: IComponent(targetObj)
{

}
void MoveController::update(const double delta)
{
	glm::vec2& pos = m_targetObj.getPosition();
	glm::vec2 offset = m_targetObj.getMoveOffset();
	double vel = m_targetObj.getVelocity();
	double weight = m_targetObj.getWeight();
	if (m_targetObj.isMove())
	{
		pos.x += static_cast<float>(delta * vel * offset.x);
		pos.y += static_cast<float>(delta * vel * offset.y);
	}	
}