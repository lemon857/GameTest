#pragma once

#include "../Engine/IComponent.h"
#include "../Engine/IGameObjec.h"

class MoveController : public IComponent
{
public:
	MoveController(IGameObject& targetObj);

	void update(const double delta) override;

private:

};