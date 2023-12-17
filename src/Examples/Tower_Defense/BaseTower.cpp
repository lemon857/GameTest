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

BaseTower::BaseTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, BaseEnemy* target, glm::vec3 pos, double cooldown, unsigned int damage, RenderEngine::Line* line)
	: m_target_BaseEnemy(std::move(target))
	, m_line(std::move(line))
	, m_cool_down(cooldown * 1000)
	, m_cur_time(0)	
	, m_damage(damage)
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
	if (m_target_BaseEnemy == nullptr)
	{		
		return;
	}
	if (m_cur_time < m_cool_down) m_cur_time += delta;
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
	if (m_target_BaseEnemy != nullptr) m_line->render_from_to(getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.f, 0.f), m_target_BaseEnemy->get_pos(), glm::vec3(1.f));
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
