#include "PhysicsEngine.h"

#include "../Game/IGameObjec.h"

double PhysicsEngine::m_g;
std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObj;

void PhysicsEngine::init()
{
	m_g = 9.8;
}
void PhysicsEngine::terminate()
{
	
}
void PhysicsEngine::update(const double delta)
{
	for (auto currentObj : m_dynamicObj)
	{
		glm::vec2& pos = currentObj->getPosition();
		glm::vec2 offset = currentObj->getOffset();
		double vel = currentObj->getVelocity();
		double weight = currentObj->getWeight();
		if (currentObj->isMove())
		{			
			pos.x += static_cast<float>(delta * vel * offset.x);
			pos.y += static_cast<float>(delta * vel * offset.y);
		}
		pos.y -= static_cast<float>(delta * weight * m_g);		
	}
}
void PhysicsEngine::addDynamicObj(std::shared_ptr<IGameObject> obj)
{
	m_dynamicObj.insert(std::move(obj));
}