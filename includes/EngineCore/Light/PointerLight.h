#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <string>
#include <vector>

class PointerLight : IGameObject
{
public:
	PointerLight(std::vector<std::string> shaders_names, float strength_light);

	void update(const double delta);

private:
	glm::vec3 m_light_color;
	std::vector<std::string> m_shaders_names;
	float m_strength_light;
};
