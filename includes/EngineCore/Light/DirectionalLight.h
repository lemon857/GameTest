#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <string>
#include <vector>

class DirectionalLight : IGameObject
{
public:
	DirectionalLight(std::vector<std::string> shaders_names);

	void on_update();

private:
	glm::vec3 m_light_color;
	std::vector<std::string> m_shaders_names;
};
