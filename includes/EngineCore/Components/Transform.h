#pragma once

#include "EngineCore/IComponent.h"

#include <glm/vec3.hpp>

class Transform : public IComponent
{
public:
	Transform(glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f), glm::vec3 rot = glm::vec3(0.f));

	void set_position(glm::vec3 pos);
	void set_scale(glm::vec3 scale);
	void set_rotation(glm::vec3 rot);

	void add_position(glm::vec3 pos);

	glm::vec3 get_position();
	glm::vec3 get_scale();
	glm::vec3 get_rotation();

private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
};