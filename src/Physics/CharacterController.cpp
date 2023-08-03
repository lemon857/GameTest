#include "CharacterController.h"

CharacterController::CharacterController(IGameObject& targetObj)
	: IComponent(targetObj)
{

}
void CharacterController::update(const double delta)
{
	glm::vec2& pos = m_targetObj.getPosition();
	glm::vec2 offset = m_targetObj.getOffset();
	double vel = m_targetObj.getVelocity();
	double weight = m_targetObj.getWeight();
	if (m_targetObj.isMove())
	{
		pos.x += static_cast<float>(delta * vel * offset.x);
		pos.y += static_cast<float>(delta * vel * offset.y);
	}	
}