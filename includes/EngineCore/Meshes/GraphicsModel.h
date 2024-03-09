#pragma once

#include "EngineCore/IGameObject.h"

#include <string>

namespace RenderEngine
{
	class Material;
}

struct GraphicsObject;
// simple renderer meshes
class GraphicsModel : public IGameObject
{
public:
	GraphicsModel(GraphicsObject* obj, RenderEngine::Material* pMaterial);

	// need additions
	void reload();
private:
	//std::string m_path;
};