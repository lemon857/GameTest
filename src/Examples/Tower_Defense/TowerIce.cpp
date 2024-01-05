#include "Games/Tower_Defense/TowerIce.h"

#include "Games/Tower_Defense/HealthBar.h"
#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/FreezeEffect.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/Sound.h"

IceTower::IceTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
	, m_time_freeze(p_time_freeze)
{
	m_sound = std::move(ResourceManager::get_unique_sound("freeze_attak"));
	m_coast = p_coast;
	m_coast_upgrade = p_coast_upgrade;
}

void IceTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance -= 3;
	m_time_freeze += 5;
	m_bar->set_max_value(m_cooldown);
	m_isUpgraded = true;
}

void IceTower::damage(BaseEnemy* target)
{
	if (!target->has_effect())
	{
		target->set_effect(std::make_unique<FreezeEffect>(m_time_freeze));
	}
}

void IceTower::target(size_t i, double dis, double& all_dis)
{
	if (dis <= m_distance && dis <= all_dis)
	{
		all_dis = dis;
		if (m_target_BaseEnemy == nullptr && !m_enemies->at(i)->has_effect())
		{
			m_target_BaseEnemy = m_enemies->at(i);
		}
		else if (m_target_BaseEnemy != nullptr)
		{
			if (m_target_BaseEnemy->has_effect()) m_target_BaseEnemy = nullptr;
		}
		//if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
	}
	else if (dis > m_distance && m_target_BaseEnemy == m_enemies->at(i))
	{
		m_target_BaseEnemy = nullptr;
	}
}

std::string IceTower::get_custom()
{
	return "Time freeze: " + std::to_string((int)m_time_freeze);
}
