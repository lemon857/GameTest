#include "Games/Tower_Defense/TowerInferno.h"

#include "Games/Tower_Defense/BaseEnemy.h"
#include "EngineCore/Sound/Sound.h"
#include "EngineCore/Sound/SoundSet.h"
#include "Games/Tower_Defense/DamageTable.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/Line.h"

InfernoTower::InfernoTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, linked_list<BaseEnemy*>* list,
	glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, P_INFERNO_COOLDOWN, p_distance, P_INFERNO_DAMAGE, line, pMaterialLine)
	, m_line_size(1)
{
	//m_sound = std::move(ResourceManager::get_unique_sound("mortar_attack"));
	m_sounds = nullptr;
	m_coast = p_coast;
	m_coast_upgrade = p_coast_upgrade;
	m_type_attack = TypeAttack::Wither;
}

void InfernoTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 4;
	m_isUpgraded = true;
}

void InfernoTower::damage(BaseEnemy* target)
{
	target->damage(m_damage);
}

void InfernoTower::updating(const double delta)
{
	if (m_target_BaseEnemy != nullptr)
	{
		m_damage += m_damage * delta * P_INFERNO_ADD;
		m_line_size += m_damage * delta * P_INFERNO_ADD * 5;
		m_line->set_size(m_line_size);
	}
	else
	{
		m_line_size = 1;
		m_line->set_size(m_line_size);
		m_damage = P_INFERNO_DAMAGE;
	}
}

void InfernoTower::rendering()
{
	if (m_target_BaseEnemy != nullptr)
	{
		m_line->render_from_to(getComponent<Transform>()->get_position() + glm::vec3(0.f, 4.5f, 0.f),
			m_target_BaseEnemy->get_pos(), glm::vec4(1.f, 0.3f, 0.1f, 1.f));
	}
}

void InfernoTower::target(size_t i, double dis, double& all_dis)
{
	if (dis <= m_distance && dis <= all_dis)
	{
		all_dis = dis;
		if (m_target_BaseEnemy == nullptr)
		{
			m_damage = P_INFERNO_DAMAGE;
			m_line_size = 1;
			m_line->set_size(m_line_size);
			m_target_BaseEnemy = m_enemies->at(i);
		}
		else if (m_target_BaseEnemy != m_enemies->at(i) && m_damage == P_INFERNO_DAMAGE)
		{
			m_target_BaseEnemy = m_enemies->at(i);
		}
	}
	else if (dis > m_distance && m_target_BaseEnemy == m_enemies->at(i))
	{
		m_damage = P_INFERNO_DAMAGE;
		m_line_size = 1;
		m_line->set_size(m_line_size);
		m_target_BaseEnemy = nullptr;
	}
}

std::string InfernoTower::get_type_str()
{
	return BaseTower::get_type_attack(TypeAttack::Wither);
}
