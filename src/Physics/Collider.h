#pragma once

#include <memory>
#include<glm/vec2.hpp>
#include "../Game/IGameObjec.h"

namespace Physics 
{

	class Collider
	{
	public:
		Collider(std::shared_ptr<IGameObject> obj)
			: m_object(obj){};
		std::shared_ptr<IGameObject> m_object;
	private:
	};
}