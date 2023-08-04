#include "Collider.h"
#include "PhysicsEngine.h"	

namespace Physics
{
	void Collider::update(const double delta)
	{

		std::string obj = "";
		if (PhysicsEngine::checkIntersection(std::make_shared<Collider>(*this), obj))
		{
			if (obj == "wall")
			{
				m_targetObj.setMoveOffset(glm::vec2(0));
				/*switch (dir)
				{
				case Physics::Up:
					if (m_moveOffset.y < 0) m_moveOffset.y = 0.f;
					break;
				case Physics::Down:
					if (m_moveOffset.y > 0) m_moveOffset.y = 0.f;
					break;
				case Physics::Left:
					if (m_moveOffset.x > 0) m_moveOffset.x = 0.f;
					break;
				case Physics::Right:
					if (m_moveOffset.x < 0) m_moveOffset.x = 0.f;
					break;
				}*/
			}
		}
	}
}