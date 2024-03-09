#pragma once

#include "EngineCore/IGameObject.h"

#include <string>
#include "glm/vec3.hpp"

namespace RenderEngine
{
	class Material;
}

class Plane : public IGameObject
{
public:
	Plane(RenderEngine::Material* pMaterial);
	Plane(RenderEngine::Material* pMaterial, glm::vec3 init_pos, glm::vec3 init_scale);
	~Plane() {}

private:
	RenderEngine::Material* m_pMaterial;
};