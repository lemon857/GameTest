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
	ObjModel(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial);

	// need additions
	void reload();
private:
	//std::string m_path;
};