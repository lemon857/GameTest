#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>
#include <string>
#include "glm/vec3.hpp"

namespace RenderEngine
{
	class Material;
}

class Plane : IGameObject
{
public:
	Plane(std::shared_ptr<RenderEngine::Material> pMaterial);
	Plane(std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 init_pos, glm::vec3 init_scale);
	~Plane() = default;

	void render() override;

private:
	std::shared_ptr<RenderEngine::Material> m_pMaterial;
};