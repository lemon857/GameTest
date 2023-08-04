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
			if (obj == "wall")
			{
				glm::vec2 offset = m_targetObj.getMoveOffset();
				switch (dir)
				{
				case Physics::Up:
					if (offset.y > 0) m_targetObj.setMoveOffset(glm::vec2(offset.x, 0.f));
					break;
				case Physics::Down:
					if (offset.y < 0) m_targetObj.setMoveOffset(glm::vec2(offset.x, 0.f));
					break;
				case Physics::Left:
					if (offset.x < 0) m_targetObj.setMoveOffset(glm::vec2(0.f, offset.y));
					break;
				case Physics::Right:
					if (offset.x > 0) m_targetObj.setMoveOffset(glm::vec2(0.f, offset.y));
					break;
				}
			}
		}
	}
}