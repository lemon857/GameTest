#include "EngineCore/Physics/Ray.h"



Ray::Ray()
	: m_from_point(glm::vec3(0.f))
	, m_to_point(glm::vec3(0.f))
	, m_direction(glm::vec3(0.f))
{

}
void Ray::set_2d_ray(glm::vec2& from_point, glm::vec2& to_point)
{
	m_from_point = glm::vec3(from_point, 0.f);
	m_to_point = glm::vec3(to_point, 2.f);
	m_direction = m_to_point - m_from_point;
}
void Ray::set_3d_ray(glm::vec3& from_point, glm::vec3& dir, float lenght)
{
	m_from_point = from_point;
	m_direction = dir;
	m_to_point = from_point + (dir * lenght);
}