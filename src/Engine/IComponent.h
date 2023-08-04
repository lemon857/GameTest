#pragma once

#include <memory>

class IGameObject;

class IComponent
{
public:

	virtual void update(const double delta) {};


protected:
	IComponent(IGameObject& targetObj)
		: m_targetObj(targetObj)
	{};
	IGameObject& m_targetObj;
};