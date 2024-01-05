#include "Games/Tower_Defense/MortarTower.h"

#include "Games/Tower_Defense/HealthBar.h"

MortarTower::MortarTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
{

}

void MortarTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 5;
	m_damage += 3;
	m_cooldown += 2000;
	m_bar->set_max_value(m_cooldown);
	m_isUpgraded = true;
}
