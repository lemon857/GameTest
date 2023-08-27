#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>

namespace RenderEngine
{
	class Material;
}

class Sprite : public IGameObject
{
public:
	Sprite(std::shared_ptr<RenderEngine::Material> pMaterial, std::string initSubTexture);

	void setSubTexture(std::string subTexture);
private:

};
