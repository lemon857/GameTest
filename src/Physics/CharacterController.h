#pragma once

#include "../Engine/IComponent.h"
#include "../Engine/IGameObjec.h"

class CharacterController : public IComponent
{
public:
	CharacterController(IGameObject& targetObj);

	void update(const double delta) override;

private:

};