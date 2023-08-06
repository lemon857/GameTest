#include "Collider.h"
#include "PhysicsEngine.h"	

#include <iostream>

namespace Physics
{
	void Collider::update(const double delta)
	{
		EDirection dir;
		std::string obj = "";
		if (PhysicsEngine::checkIntersection(std::make_shared<Collider>(*this), obj, dir))
		{
			if (m_onCollisionCallback)
			{
				m_onCollisionCallback(m_targetObj, obj, dir);
			}
		}
	}
	void Collider::setOnCollisionCallback(std::function<void(IGameObject&, std::string, Physics::EDirection)> callback)
	{
		m_onCollisionCallback = callback;
	}
}