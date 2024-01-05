#pragma once

#include "Games/Tower_Defense/BaseTower.h"

namespace RenderEngine
{
	class Circle;
}

class MortarTower : public BaseTower
{
public:
	MortarTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, 
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~MortarTower() = default;

	void upgrade() override;
	void rendering() override;
	void damage(BaseEnemy* target) override;
	void updating(const double delta) override;
	std::string get_custom() override;

	static const unsigned int p_damage = 6;
	static const unsigned int p_distance = 20;
	static const unsigned int p_cooldown = 14;
	static const unsigned int p_radius_attack = 2;
	static const unsigned int p_coast = 30;
	static const unsigned int p_coast_upgrade = 13;

	std::string get_add_cooldown() override { return " +2"; }
	std::string get_add_distance() override { return " +5"; }
	std::string get_add_damage() override { return " +1"; }
	std::string get_add_custom() override { return " +1"; };

	static std::string get_type_str();
private:

	unsigned int m_radius;

	double m_cur_time;
	double m_dur_time;
	bool m_isActiveCircle;

	RenderEngine::Circle* m_attack;

	std::unique_ptr<Sound> m_sound_attack;
};