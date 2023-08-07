#pragma once

#include <memory>
#include <functional>
#include<glm/vec2.hpp>
#include "../Engine/IGameObjec.h"
#include "../Engine/IComponent.h"
#include "../Physics/PhysicsEngine.h"

namespace Physics 
{

	class Collider : public IComponent
	{
	public:
		Collider(IGameObject& targetObj)
			: IComponent(targetObj)
		{};

		void update(const double delta) override;
		// Каллбэк который вызовет функцию при касании и передаст в неё имя объекта
		void setOnCollisionCallback(std::function<void(IGameObject&, IGameObject&, Physics::EDirection)> callback);

		glm::vec2& getSize() { return m_targetObj.getSize(); };
		glm::vec2& getPosition() { return m_targetObj.getPosition(); };
		IGameObject& getObject() { return m_targetObj; }
	private:
		std::function<void(IGameObject&, IGameObject&, Physics::EDirection)> m_onCollisionCallback;
	};
}