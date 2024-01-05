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

	double get_vel() { return m_velocity; }
	void set_vel(const double vel) { if (vel >= 0) m_velocity = vel; else m_velocity = 0; }

	glm::vec3 get_pos();

	void set_effect(std::unique_ptr<BaseEffect> effect);

	bool has_effect() { return m_has_effect; }

	void set_color_effect(glm::vec3 colorFore, glm::vec3 colorBack);

	unsigned int get_reward() { return m_reward; }

protected:

	std::vector<Target> m_targets;
	int curtarget;

	Castle* m_target_castle;
	ObjModel* m_model;
	HealthBar* m_bar;
	HealthBar* m_bar_effect;
	std::unique_ptr<BaseEffect> m_effect;

	int m_hp;
	unsigned int m_dmg;
	double m_cur_time;

	unsigned int m_reward = 2;
	double m_cool_down;

	double m_velocity;
	bool m_isDestroyed;
	bool m_has_effect;
};