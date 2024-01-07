#include "Games/Tower_Defense/TowerMortar.h"

#include "Games/Tower_Defense/HealthBar.h"
#include "Games/Tower_Defense/BaseEnemy.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/Sound.h"
#include "EngineCore/Renderer/Circle.h"
#include "Games/Tower_Defense/DamageTable.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/Line.h"

MortarTower::MortarTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial,
	linked_list<BaseEnemy*>* list, glm::vec3 pos, RenderEngine::Line* line, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseTower(obj, pMaterial, std::move(list), pos, p_cooldown, p_distance, p_damage, line, pMaterialLine)
	, m_radius(p_radius_attack)
	, m_dur_time(2000)
	, m_cur_time(0)
	, m_isActiveCircle(false)
{
	m_sound = std::move(ResourceManager::get_unique_sound("mortar_attack"));
	m_sound_attack = std::move(ResourceManager::get_unique_sound("mortar_fall"));
	m_attack = new RenderEngine::Circle(pMaterialLine, pos - glm::vec3(0.f, 100.f, 0.f), glm::vec3(1.f), glm::vec3(0.f),
		glm::vec4(1.f, 0.f, 0.3f, 0.7f), m_radius, 90, 3);
	m_coast = p_coast;
	m_coast_upgrade = p_coast_upgrade;
	m_type_attack = TypeAttack::Shock;
}

void MortarTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 5;
	m_damage += 1;
	m_cooldown += 2000;
	m_radius += 1;
	m_attack->update(m_radius, 90);
	m_bar->set_max_value(m_cooldown);
	m_isUpgraded = true;
}

void MortarTower::rendering()
{
	m_attack->render(); 
	if (m_target_BaseEnemy != nullptr)
	{
		m_line->render_from_to(getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.f, 0.f),
			m_target_BaseEnemy->get_pos(), glm::vec4(1.f, 0.3f, 0.7f, 1.f));
	}
}

void MortarTower::damage(BaseEnemy* target)
{
	glm::vec3 pos = target->get_pos();
	m_isActiveCircle = true;
	m_attack->set_pos(pos);
	for (size_t i = 0; i < m_enemies->size(); i++)
	{
		if (m_enemies->at(i) == nullptr) continue;
		glm::vec3 p = m_enemies->at(i)->get_pos();
		if (p == pos) continue;
		if (((p.x - pos.x) * (p.x - pos.x)) + ((p.z - pos.z) * (p.z - pos.z)) < m_radius * m_radius)
		{
			m_sound_attack->play();
			m_enemies->at(i)->damage(m_damage);
		}
	}
}

void MortarTower::updating(const double delta)
{
	if (!m_isActiveCircle) return;
	if (m_cur_time < m_dur_time) m_cur_time += delta;
	else
	{
		m_attack->set_pos(get_pos() - glm::vec3(0.f, 100.f, 0.f));
		m_isActiveCircle = false;
		m_cur_time = 0;
	}		
}

std::string MortarTower::get_custom()
{
	return "Radius attack: " + std::to_string(m_radius);
}

std::string MortarTower::get_type_str()
{
	return BaseTower::get_type_attack(TypeAttack::Shock);
}
