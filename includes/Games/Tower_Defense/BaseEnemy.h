#pragma once

#include "Games/Tower_Defense/Target.h"

#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>

#include <string>
#include <memory>
#include <vector>

class Castle;
class HealthBar;
class BaseEffect;
enum class TypeArmor;

namespace RenderEngine
{
	class Material;
}

class BaseEnemy : public IGameObject
{
public:
	BaseEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial, Castle* target, std::vector<Target> targets, glm::vec3 pos, const double cooldown, const double velocity,
		const unsigned int maxHP, const double damage, std::shared_ptr<RenderEngine::Material> pMaterialLine, glm::vec3 color = glm::vec3(1.f, 0.f, 0.f));
	~BaseEnemy();

	void update(const double delta) override;

	void render() override;

	virtual void on_render() {};

	virtual void on_update(const double delta) {};

	void damage(const double damage_hp);
	
	void destroy() { m_isDestroyed = true; }

	bool is_destroy() { return m_isDestroyed; }
	bool is_broken() { return m_isBroken; }

	double get_cooldown() { return m_cooldown / 1000.0; }
	double get_vel() { return m_velocity * 1000; }
	void set_vel(const double vel) { if (vel >= 0) m_velocity = vel; else m_velocity = 0; }

	void set_scale(glm::vec3 scale);

	glm::vec3 get_pos();

	void set_effect(std::unique_ptr<BaseEffect> effect);

	bool has_effect() { return m_has_effect; }

	void set_color_effect(glm::vec3 colorFore, glm::vec3 colorBack);

	float get_distance() { return m_distance_attack; }
	double get_hp() { return m_hp; }
	double get_max_hp() { return m_max_hp; }
	double get_damage() { return m_dmg; }
	std::wstring get_description() { return m_description; }

	void set_desc(std::wstring desc) { m_description = desc; }

	unsigned int get_reward() { return m_reward; }

	TypeArmor get_type() { return m_type_armor; }

	static std::wstring get_type_str(TypeArmor type);

protected:
	std::wstring m_description;
	std::vector<Target> m_targets;
	int curtarget;
	TypeArmor m_type_armor;
	Castle* m_target_castle;
	HealthBar* m_bar;
	HealthBar* m_bar_effect;
	std::unique_ptr<BaseEffect> m_effect;
	glm::vec3 m_last_dir;
	double m_rotation_break;

	IGameObject* m_obj;

	double m_hp;
	double m_max_hp;
	double m_dmg;
	double m_cur_time;

	float m_distance_attack;
	int m_stop_target_walking;

	unsigned int m_reward = 2;
	double m_cooldown;

	double m_velocity;
	bool m_isDestroyed;
	bool m_isBroken;
	bool m_has_effect;
};