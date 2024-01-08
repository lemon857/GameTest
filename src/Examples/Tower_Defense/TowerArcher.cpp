#include "Games/Tower_Defense/TowerArcher.h"

#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/SoundSet.h"
#include "EngineCore/Sound/Sound.h"
#include "Games/Tower_Defense/DamageTable.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/Line.h"

ArcherTower::ArcherTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, 
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
{
	m_sounds->add_sound(std::move(ResourceManager::get_unique_sound("bow_attack")));
	m_coast_upgrade = p_coast_upgrade;
	m_coast = p_coast;
	m_type_attack = TypeAttack::Piercing;
	m_start_dis = 0;
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

void ArcherTower::rendering()
{
	if (m_target_BaseEnemy != nullptr)
	{
		m_line->render_from_to(getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.5f, 0.f),
			m_target_BaseEnemy->get_pos(), glm::vec4(0.5f, 0.3f, 0.7f, 1.f));
	}
}

void ArcherTower::target(size_t i, double dis, double& all_dis)
{
	if (dis <= m_distance && dis >= all_dis)
	{
		all_dis = dis;
		if (m_target_BaseEnemy == nullptr)
		{
			m_target_BaseEnemy = m_enemies->at(i);
		}
		else if (m_target_BaseEnemy != m_enemies->at(i))
		{
			m_target_BaseEnemy = m_enemies->at(i);
		}
	}
	else if (dis > m_distance && m_target_BaseEnemy == m_enemies->at(i))
	{
		m_target_BaseEnemy = nullptr;
	}
}

std::string ArcherTower::get_type_str()
{
	return BaseTower::get_type_attack(TypeAttack::Piercing);
}
