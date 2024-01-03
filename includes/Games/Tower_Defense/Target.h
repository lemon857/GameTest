#pragma once

#include <glm/vec3.hpp>

class Target
{
public:
	Target(glm::vec3 pos)
		: m_pos(pos)
	{

	}
	~Target() = default;

	virtual glm::vec3 get_pos()
	{
		return m_pos;
	}

private:
	glm::vec3 m_pos;
};