#pragma once

#include "Games/Tower_Defense/BaseTower.h"

class ArcherTower : public BaseTower
{
public:
	ArcherTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~ArcherTower() = default;

	void upgrade() override;
	void rendering() override;
	void target(size_t i, double dis, double& all_dis) override;

	static const unsigned int p_damage = 3;
	static const unsigned int p_distance = 10;
	static const unsigned int p_cooldown = 1;
	static const unsigned int p_coast = 15;
	static const unsigned int p_coast_upgrade = 7;

	std::wstring get_add_cooldown() override { return L" +1"; }
	std::wstring get_add_distance() override { return L" +3"; }
	std::wstring get_add_damage() override { return L" +1"; }

	static std::wstring get_type_str();
private:

};