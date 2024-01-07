#pragma once

#include "EngineCore/IComponent.h"
#include "EngineCore/IGameObject.h"

class MoveController : public IComponent
{
public:
	MoveController(IGameObject& targetObj);

	void update(const double delta);

private:

};