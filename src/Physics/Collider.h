#pragma once

#include <memory>
#include<glm/vec2.hpp>
#include "../Engine/IGameObjec.h"
#include "../Engine/IComponent.h"

namespace Physics 
{

	class Collider : public IComponent
	{
	public:
		Collider(IGameObject& targetObj)
			: IComponent(targetObj)
		{};
		glm::vec2& getSize() { return m_targetObj.getSize(); };
		glm::vec2& getPosition() { return m_targetObj.getPosition(); };
		IGameObject& getObject() { return m_targetObj; }
	private:
	};
}