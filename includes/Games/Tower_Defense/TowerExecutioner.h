#pragma once

#include "Games/Tower_Defense/BaseTower.h"

class ExecutionerTower : public BaseTower
{
public:
	ExecutionerTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~ExecutionerTower() = default;

	void upgrade() override;
	void rendering() override;

	static const unsigned int p_damage = 4;
	static const unsigned int p_distance = 13;
	static const unsigned int p_cooldown = 5;
	static const unsigned int p_coast = 20;
	static const unsigned int p_coast_upgrade = 12;

	std::wstring get_add_cooldown() override { return L" -1"; }
	std::wstring get_add_distance() override { return L" +3"; }
	std::wstring get_add_damage() override { return L" +2"; }

	static std::wstring get_type_str();
private:
};