#include "Games/Tower_Defense/BaseTower.h"

#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/BaseEffect.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Resources/ResourceManager.h"

#include "EngineCore/Sound/Sound.h"

#include <glm/gtc/matrix_transform.hpp>

BaseTower::BaseTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, linked_list<BaseEnemy*>* list, glm::vec3 pos, double cooldown, double distance, unsigned int damage, RenderEngine::Line* line)
	: m_target_BaseEnemy(nullptr)
	, m_enemies(std::move(list))
	, m_line(std::move(line))
	, m_cooldown(cooldown * 1000)
	, m_distance(distance)
	, m_cur_time(0)	
	, m_damage(damage)
	, m_isUpgraded(false)
	, IGameObject("Tower")
{
	addComponent<Transform>(pos);
	addComponent<MeshRenderer>(std::move(obj), pMaterial);
	m_sound = std::move(ResourceManager::get_unique_sound("tower_attak"));
}

BaseTower::~BaseTower()
{
	m_sound->terminate();
}

void BaseTower::update(const double delta)
{
	double distance = 100; // <-------
	for (size_t i = 0; i < m_enemies->size(); i++)
	{
		if (m_enemies->at(i) == nullptr) continue;
		glm::vec3 a = m_enemies->at(i)->get_pos() - get_pos();
		double d = sqrt(a.x * a.x + a.z * a.z);
		if (d <= m_distance && d <= distance)
		{
			distance = d;
			if (m_target_BaseEnemy == nullptr)
			{
				m_target_BaseEnemy = m_enemies->at(i);
			}
			//if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
		}
		else if (d > m_distance)
		{
			m_target_BaseEnemy = nullptr;
		}
	}
	if (m_target_BaseEnemy == nullptr)
	{		
		return;
	}
	if (m_cur_time < m_cooldown) m_cur_time += delta;
	else
	{
		if (m_target_BaseEnemy != nullptr)
		{
			m_sound->play();
			m_target_BaseEnemy->damage(m_damage);
		}
		m_cur_time = 0;
	}
}

void BaseTower::render()
{
	updateComponents(0);
	if (m_target_BaseEnemy != nullptr) m_line->render_from_to(getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.f, 0.f),
		m_target_BaseEnemy->get_pos(), glm::vec4(1.f, 0.1f, 0.1f, 1.f));
}

void BaseTower::upgrade()
{
	if (m_isUpgraded) return;
	m_distance += 3;
	m_damage += 5;
	m_cooldown -= 1000;
	m_isUpgraded = true;
}

void BaseTower::set_target(BaseEnemy* target)
{
	m_target_BaseEnemy = target;
	//if (target != nullptr) m_target_BaseEnemy->set_effect(new BaseEffect(30, 1, 6));
}

glm::vec3 BaseTower::get_pos()
{
	return getComponent<Transform>()->get_position();
}

BaseEnemy* BaseTower::get_target()
{
	return m_target_BaseEnemy;
}
