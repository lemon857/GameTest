#pragma once

#include <memory>

class IGameObject;

class IComponent
{
public:
	IComponent(IGameObject& targetObj)
		: m_targetObj(targetObj)
	{};

	virtual void update(double delta) {};


protected:
	IGameObject& m_targetObj;
};