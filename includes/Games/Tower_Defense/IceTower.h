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

	static const unsigned int p_damage = 1;
	static const unsigned int p_distance = 14;
	static const unsigned int p_cooldown = 3;

private:

};