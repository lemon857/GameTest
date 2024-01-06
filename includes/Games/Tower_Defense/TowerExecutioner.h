#pragma once

#include "Games/Tower_Defense/BaseTower.h"

#define P_EXECUTIONER_DESCRIPTION "Just cutting"

class ExecutionerTower : public BaseTower
{
public:
	ExecutionerTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~ExecutionerTower() = default;

	void upgrade() override;

	static const unsigned int p_damage = 4;
	static const unsigned int p_distance = 13;
	static const unsigned int p_cooldown = 5;
	static const unsigned int p_coast = 20;
	static const unsigned int p_coast_upgrade = 12;

	std::string get_add_cooldown() override { return " -1"; }
	std::string get_add_distance() override { return " +3"; }
	std::string get_add_damage() override { return " +2"; }
	std::string get_description() override { return P_EXECUTIONER_DESCRIPTION; }

	static std::string get_type_str();
private:
};