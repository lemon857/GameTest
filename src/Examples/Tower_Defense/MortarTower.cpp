#include "Games/Tower_Defense/MortarTower.h"

MortarTower::MortarTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line)
{

}

void MortarTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 5;
	m_damage += 3;
	m_cooldown += 1000;
	m_isUpgraded = true;
}
