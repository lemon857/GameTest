#include "EngineCore/Components/Transform.h"

Transform::Transform(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
	: m_position(pos)
	, m_scale(scale)
	, m_rotation(rot)
	, IComponent()
{
}

void Transform::set_position(glm::vec3 pos)
{
	m_position = pos;
}

void Transform::set_scale(glm::vec3 scale)
{
	m_scale = scale;
}

void Transform::set_rotation(glm::vec3 rot)
{
	m_rotation = rot;
}

glm::vec3 Transform::get_position()
{
	return m_position;
}

glm::vec3 Transform::get_scale()
{
	return m_scale;
}

glm::vec3 Transform::get_rotation()
{
	return m_rotation;
}