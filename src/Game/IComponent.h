#pragma once

#include <memory>

class IGameObject;

class IComponent
{
public:
	IComponent(std::shared_ptr<IGameObject>& targetObj)
		: m_targetObj(std::move(targetObj))
	{};

	virtual void update(double delta) {};


protected:
	std::shared_ptr<IGameObject> m_targetObj;
};