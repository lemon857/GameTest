#pragma once

#include "Games/Tower_Defense/BaseTower.h"

class IceTower : public BaseTower
{
public:
	IceTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~IceTower() = default;

	void upgrade() override;

	void damage(BaseEnemy* target) override;

	void target(size_t i, double dis, double& all_dis) override;

	std::string get_custom() override;

	static const unsigned int p_damage = 1;
	static const unsigned int p_distance = 16;
	static const unsigned int p_cooldown = 3;
	static const unsigned int p_time_freeze = 10;

private:
	double m_time_freeze;
};