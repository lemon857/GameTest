#pragma once

#include "EngineCore/IGameObject.h"

#include <string>
#include <memory>

namespace RenderEngine
{
	class Material;
}

struct GraphicsObject;

class ObjModel : public IGameObject
{
public:
	ObjModel(const std::string relativeFilePath, std::shared_ptr<RenderEngine::Material> pMaterial);

	void reload();
private:
	std::string m_path;
};