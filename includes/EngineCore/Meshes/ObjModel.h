#pragma once

#include "EngineCore/IGameObject.h"

#include <string>
#include <memory>

namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;
}

struct GraphicsObject;

class ObjModel : public IGameObject
{
public:
	ObjModel(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Texture2D> pTexture, std::shared_ptr<RenderEngine::ShaderProgram> pShader);
private:
	std::shared_ptr<RenderEngine::Texture2D> m_pTexture;
};