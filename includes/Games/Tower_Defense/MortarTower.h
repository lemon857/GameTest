#pragma once

#include "Games/Tower_Defense/BaseTower.h"

class MortarTower : public BaseTower
{
public:
	MortarTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, 
		linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line);
	~MortarTower() = default;

	void upgrade() override;

	static const unsigned int p_damage = 10;
	static const unsigned int p_distance = 20;
	static const unsigned int p_cooldown = 7;

private:

};