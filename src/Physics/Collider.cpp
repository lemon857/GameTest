#include "EngineCore/Physics/Collider.h"
#include "EngineCore/Physics/PhysicsEngine.h"	

#include <iostream>

namespace Physics
{
	void Collider::update(const double delta)
	{
		EDirection dir;
		IGameObject* obj = PhysicsEngine::checkIntersection(std::make_shared<Collider>(), dir);
		if (obj)
		{
			if (m_onCollisionCallback)
			{
				//m_onCollisionCallback(m_targetObj, *obj, dir);
			}
		}
	}
	void Collider::setOnCollisionCallback(onCollisionCallback callback)
	{
		m_onCollisionCallback = callback;
	}
}