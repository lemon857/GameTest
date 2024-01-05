#include "Games/Tower_Defense/ArcherTower.h"

#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/Sound.h"

ArcherTower::ArcherTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, 
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
{
	m_sound = std::move(ResourceManager::get_unique_sound("bow_attack"));
	m_coast_upgrade = p_coast_upgrade;
	m_coast = p_coast;
}

void ArcherTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 3;
	m_damage += 1;
	m_cooldown += 1000;
	m_bar->set_max_value(m_cooldown);
	m_isUpgraded = true;
}
