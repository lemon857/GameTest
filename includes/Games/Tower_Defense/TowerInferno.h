#pragma once

#include "Games/Tower_Defense/BaseTower.h"

#define P_INFERNO_DESCRIPTION "Increases damage over time with one enemy"
#define P_INFERNO_COOLDOWN 0.2
#define P_INFERNO_DAMAGE 0.1
#define P_INFERNO_ADD 0.0003

class InfernoTower : public BaseTower
{
public:
	InfernoTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~InfernoTower() = default;

	void upgrade() override;
	void damage(BaseEnemy* target) override;
	void updating(const double delta) override;
	void target(size_t i, double dis, double& all_dis) override;

	static const unsigned int p_distance = 14;
	static const unsigned int p_coast = 35;
	static const unsigned int p_coast_upgrade = 15;

	std::string get_add_distance() override { return " +4"; }
	std::string get_description() override { return P_INFERNO_DESCRIPTION; }

	static std::string get_type_str();
private:

	double m_cur_time;

	std::unique_ptr<Sound> m_sound_attack;
};