#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>
#include <string>

namespace RenderEngine
{
	class Material;
}

class Plane : IGameObject
{
public:
	Plane(std::shared_ptr<RenderEngine::Material> pMaterial);
	~Plane() = default;

private:
	std::shared_ptr<RenderEngine::Material> m_pMaterial;
};