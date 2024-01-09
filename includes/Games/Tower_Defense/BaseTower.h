#pragma once

#include "EngineCore/IGameObject.h"
#include "EngineCore/System/List.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>


class HealthBar;
class ObjModel;
class SoundSet;
class BaseEnemy;
enum class TypeAttack;

namespace RenderEngine
{
	class Material;
	class Line;
}

struct GraphicsObject;

class BaseTower : public IGameObject
{
public:
	BaseTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, linked_list<BaseEnemy*>* list, glm::vec3 pos,
		double cooldown, double distance, unsigned int damage, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~BaseTower();

	virtual void update(const double delta) override;

	void render() override;

	virtual void upgrade();
	virtual void damage(BaseEnemy* target) {}
	virtual void rendering() {}
	virtual void updating(const double delta) {}
	virtual void target(size_t i, double dis, double& all_dis);

	void set_target(BaseEnemy* target);

	glm::vec3 get_pos();

	BaseEnemy* get_target();

	bool is_upgradable() {	return !m_isUpgraded; }

	double get_cooldown() { return m_cooldown / 1000.0; }
	double get_distance() { return m_distance; }
	double get_damage() { return m_damage; }
	unsigned int get_coast() { return m_coast; }
	unsigned int get_upgrade_coast() { return m_coast_upgrade; }
	virtual std::wstring get_custom() { return L""; };
	std::wstring get_description() { return m_description; }

	virtual std::wstring get_add_cooldown() { return L""; }
	virtual std::wstring get_add_distance() { return L""; }
	virtual std::wstring get_add_damage() { return L""; }
	virtual std::wstring get_add_custom() { return L""; };

	static std::wstring get_type_attack(TypeAttack type);

	std::wstring get_self_type_str();

	void set_desc(std::wstring desc);

protected:
	std::wstring m_description;

	BaseEnemy* m_target_BaseEnemy;

	linked_list<BaseEnemy*>* m_enemies;

	HealthBar* m_bar;

	TypeAttack m_type_attack;

	double m_cur_time;

	double m_cooldown;
	double m_distance;

	double m_start_dis;

	double m_damage;
	unsigned int m_coast;
	unsigned int m_coast_upgrade;

	RenderEngine::Line* m_line;

	std::unique_ptr<SoundSet> m_sounds;

	bool m_isUpgraded;
};