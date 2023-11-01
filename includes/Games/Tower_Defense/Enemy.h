#pragma once

#define MIN_DISTANCE_TO_CASTLE 2.7f

#include <glm/vec3.hpp>

#include <memory>
#include <functional>

class Castle;
class ObjModel;
class HealthBar;

namespace RenderEngine
{
	class Material;
}


class Enemy
{
public:
	Enemy(ObjModel* model, Castle* target, glm::vec3 pos, double cooldown, double velocity, const unsigned int maxHP, std::shared_ptr<RenderEngine::Material> pMaterial);
	~Enemy();

	void update(const double delta);

	void damage(const unsigned int damage_hp);
	
	bool is_destroy() { return m_isDestroyed; }

	glm::vec3 get_pos();

	void set_angle(double* angle);

private:

	Castle* m_target_castle;
	ObjModel* m_model;
	HealthBar* m_bar;

	double* m_angle;

	unsigned int m_hp;

	double m_cur_time;

	double m_cool_down;

	double m_velocity;
	bool m_isDestroyed;
};