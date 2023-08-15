#pragma once

#include<glm/vec3.hpp>
#include<glm/vec2.hpp>

class Ray
{
public:
	Ray();

	void set_2d_ray(glm::vec2& from_point, glm::vec2& to_point);
	void set_3d_ray(glm::vec3& from_point, glm::vec3& dir, float lenght);

	glm::vec3 get_from_point() { return m_from_point; };
	glm::vec3 get_to_point() { return m_to_point; };
	glm::vec3 get_direction() { return m_direction; };

private:
	glm::vec3 m_from_point;
	glm::vec3 m_to_point; 
	glm::vec3 m_direction;
};