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
	ObjModel(const std::string relativeFilePath, std::shared_ptr<RenderEngine::Texture2D> pTexture, std::shared_ptr<RenderEngine::ShaderProgram> pShader);
	void reload();
private:
	std::shared_ptr<RenderEngine::Texture2D> m_pTexture;
	std::string m_path;
};