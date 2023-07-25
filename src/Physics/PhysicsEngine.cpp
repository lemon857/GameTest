#include "PhysicsEngine.h"

#include "../Game/IGameObjec.h"
#include "Collider.h"

namespace Physics
{
	int PhysicsEngine::m_currentID = 0;
	double PhysicsEngine::m_g;
	bool PhysicsEngine::m_useGravity;
	PhysicsEngine::objMap PhysicsEngine::m_dynamicObj;
	PhysicsEngine::ColliderMap PhysicsEngine::m_colliders;

	void PhysicsEngine::init(bool useGravity)
	{
		m_useGravity = useGravity;
		m_g = 9.8;
	}
	void PhysicsEngine::terminate()
	{

	}
	void PhysicsEngine::update(const double delta)
	{
		for (auto currentObj : m_dynamicObj)
		{
			glm::vec2& pos = currentObj.second->getPosition();
			glm::vec2 offset = currentObj.second->getOffset();
			double vel = currentObj.second->getVelocity();
			double weight = currentObj.second->getWeight();
			if (currentObj.second->isMove())
			{
				pos.x += static_cast<float>(delta * vel * offset.x);
				pos.y += static_cast<float>(delta * vel * offset.y);
			}
			//if (!currentObj.second->getKinematicState() && !currentObj.second->getGroundState()) pos.y -= static_cast<float>(delta * weight * m_g);
			//ColliderMap::const_iterator it1 = m_colliders.find(1);
			//ColliderMap::const_iterator it2 = m_colliders.find(2);
			//currentObj.second->getGroundState(hasCollidersIntersection(it1->second, it2->second));
		}
	}
	void PhysicsEngine::addDynamicObj(std::shared_ptr<IGameObject> obj, int id)
	{
		m_dynamicObj.emplace(id, std::move(obj));
	}
	void PhysicsEngine::addCollider(std::shared_ptr<Collider> collider, int id)
	{
		m_colliders.emplace(id, std::move(collider));
	}
	bool PhysicsEngine::hasCollidersIntersection(const std::shared_ptr<Collider> collider1,	const std::shared_ptr<Collider> collider2)
	{
		const glm::vec2 collider1_bottomLeft_world = collider1->m_object->getPosition();
		const glm::vec2 collider1_topRight_world = collider1->m_object->getPosition() + collider1->m_object->getSize();

		const glm::vec2 collider2_bottomLeft_world = collider2->m_object->getPosition();
		const glm::vec2 collider2_topRight_world = collider2->m_object->getPosition() + collider1->m_object->getSize();

		if (collider1_bottomLeft_world.x >= collider2_topRight_world.x)
		{
			return false;
		}
		if (collider1_topRight_world.x <= collider2_bottomLeft_world.x)
		{
			return false;
		}

		if (collider1_bottomLeft_world.y >= collider2_topRight_world.y)
		{
			return false;
		}
		if (collider1_topRight_world.y <= collider2_bottomLeft_world.y)
		{
			return false;
		}

		return true;
	}
}