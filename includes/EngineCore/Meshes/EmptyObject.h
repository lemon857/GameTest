#pragma once

#include "EngineCore/IGameObject.h"

static int g_current_emptyobject_ID = 0;

class EmptyObject : public IGameObject
{
public:
	EmptyObject()
		: IGameObject("Empty object" + std::to_string(g_current_emptyobject_ID++))
	{};

private:

};