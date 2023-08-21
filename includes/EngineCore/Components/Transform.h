#pragma once

#include "EngineCore/IComponent.h"

#include <glm/vec3.hpp>

class Transform : public IComponent
{
public:
	Transform();

	void set_position(glm::vec3 pos);
	void set_scale(glm::vec3 scale);
	void set_rotation(glm::vec3 rot);

	glm::vec3 get_position();
	glm::vec3 get_scale();
	glm::vec3 get_rotation();

private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
};