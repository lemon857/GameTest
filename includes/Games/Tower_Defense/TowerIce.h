#pragma once

#include "Games/Tower_Defense/BaseTower.h"

class IceTower : public BaseTower
{
public:
	IceTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~IceTower() = default;

	void upgrade() override;

	void rendering() override;

	void damage(BaseEnemy* target) override;

	void target(size_t i, double dis, double& all_dis) override;

	std::wstring get_custom() override;

	static const unsigned int p_damage = 1;
	static const unsigned int p_distance = 16;
	static const unsigned int p_cooldown = 3;
	static const unsigned int p_time_freeze = 10;
	static const unsigned int p_coast = 24;
	static const unsigned int p_coast_upgrade = 11;

	std::wstring get_add_distance() override { return L" -3"; }
	std::wstring get_add_custom() override { return L" +5"; };

	static std::wstring get_type_str();
private:
	double m_time_freeze;
};