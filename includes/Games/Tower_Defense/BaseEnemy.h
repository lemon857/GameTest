#pragma once

#define MIN_DISTANCE_TO_CASTLE 2.7f

#include <glm/vec3.hpp>

#include <memory>
#include <vector>

class Castle;
class ObjModel;
class HealthBar;
class BaseEffect;
class Target;

namespace RenderEngine
{
	class Material;
}

class BaseEnemy
{
public:
	BaseEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, const double cooldown, const double velocity,
		const unsigned int maxHP, const unsigned int damage, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 color = glm::vec3(1.f, 0.f, 0.f));
	~BaseEnemy();

	void update(const double delta);

	void render();

	void damage(const unsigned int damage_hp);
	
	bool is_destroy() { return m_isDestroyed; }

	glm::vec3 get_pos();

	void set_effect(BaseEffect* effect);

protected:

	std::vector<Target> m_targets;
	int curtarget;

	Castle* m_target_castle;
	ObjModel* m_model;
	HealthBar* m_bar;
	BaseEffect* m_effect;

	int m_hp;
	unsigned int m_dmg;
	double m_cur_time;

	double m_cool_down;

	double m_velocity;
	bool m_isDestroyed;
};