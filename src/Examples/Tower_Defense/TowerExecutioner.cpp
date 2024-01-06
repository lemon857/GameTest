#include "Games/Tower_Defense/TowerExecutioner.h"

#include "Games/Tower_Defense/HealthBar.h"
#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/FreezeEffect.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/Sound.h"
#include "Games/Tower_Defense/DamageTable.h"

ExecutionerTower::ExecutionerTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
{
	m_sound = std::move(ResourceManager::get_unique_sound("executioner_attack"));
	m_coast = p_coast;
	m_coast_upgrade = p_coast_upgrade;
	m_type_attack = TypeAttack::Cutting;
}

void ExecutionerTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 3;
	m_damage += 2;
	m_cooldown -= 1000;
	m_bar->set_max_value(m_cooldown);
	m_isUpgraded = true;
}

std::string ExecutionerTower::get_type_str()
{
	return BaseTower::get_type_attack(TypeAttack::Cutting);
}
