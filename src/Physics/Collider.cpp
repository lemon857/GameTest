#include "Collider.h"
#include "PhysicsEngine.h"	

#include <iostream>

namespace Physics
{
	void Collider::update(const double delta)
	{
		EDirection dir;
		IGameObject* obj = PhysicsEngine::checkIntersection(std::make_shared<Collider>(*this), dir);
		if (obj)
		{
			if (m_onCollisionCallback)
			{
				m_onCollisionCallback(m_targetObj, *obj, dir);
			}
		}
	}
	void Collider::setOnCollisionCallback(std::function<void(IGameObject&, IGameObject&, Physics::EDirection)> callback)
	{
		m_onCollisionCallback = callback;
	}
}