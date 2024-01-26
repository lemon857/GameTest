#pragma once

#include "EngineCore/IGameObject.h"

#include <string>
#include <memory>

namespace RenderEngine
{
	class Material;
}

struct GraphicsObject;
// simple renderer meshes
class GraphicsModel : public IGameObject
{
public:
	GraphicsModel(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial);

	// need additions
	void reload();
private:
	//std::string m_path;
};