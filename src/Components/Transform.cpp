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

void Transform::add_position(glm::vec3 pos)
{
	m_position += pos;
}

void Transform::add_rotation(glm::vec3 rot)
{
	m_rotation += rot;
}


void Transform::set_position_x(float pos)
{
	m_position.x = pos;
}
void Transform::set_position_y(float pos)
{
	m_position.y = pos;
}
void Transform::set_position_z(float pos)
{
	m_position.z = pos;
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