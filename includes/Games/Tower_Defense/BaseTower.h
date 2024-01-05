#pragma once

#include "EngineCore/IGameObject.h"
#include "EngineCore/System/List.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

class HealthBar;
class BaseEnemy;
class ObjModel;
class Sound;

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

	double get_cooldown() { return m_cooldown / 1000.0; }
	double get_distance() { return m_distance; }
	unsigned int get_damage() { return m_damage; }
	virtual std::string get_custom() { return ""; };
	
protected:
	BaseEnemy* m_target_BaseEnemy;

	linked_list<BaseEnemy*>* m_enemies;

	HealthBar* m_bar;

	double m_cur_time;

	double m_cooldown;
	double m_distance;

	unsigned int m_damage;

	RenderEngine::Line* m_line;

	std::unique_ptr<Sound> m_sound;

	bool m_isUpgraded;
};