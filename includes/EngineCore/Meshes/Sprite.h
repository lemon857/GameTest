#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>

namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;
}

class Sprite : public IGameObject
{
public:
	Sprite(std::shared_ptr<RenderEngine::Texture2D> pTexture, std::string initSubTexture, std::shared_ptr<RenderEngine::ShaderProgram> pShader);

	void setSubTexture(std::string subTexture);
private:

};
