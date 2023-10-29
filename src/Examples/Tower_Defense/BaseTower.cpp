#include "Games/Tower_Defense/BaseTower.h"

#include "Games/Tower_Defense/Enemy.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Resources/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

BaseTower::BaseTower(std::string objPath, std::shared_ptr<RenderEngine::Material> pMaterial, Enemy* target, glm::vec3 pos, double cooldown, RenderEngine::Line* line)
	: m_target_enemy(std::move(target))
	, m_line(std::move(line))
	, m_cool_down(cooldown * 1000)
	, m_cur_time(0)	
	, IGameObject("Tower")
{
	addComponent<Transform>(pos);
	addComponent<MeshRenderer>(ResourceManager::load_OBJ_file(objPath), pMaterial);
}

BaseTower::~BaseTower()
{

}

void BaseTower::update(const double delta)
{
	updateComponents(delta);
	if (m_target_enemy == nullptr)
	{		
		return;
	}
	if (m_cur_time < m_cool_down) m_cur_time += delta;
	else
	{
		if (m_target_enemy != nullptr) m_target_enemy->damage(5);
		m_cur_time = 0;
	}
	m_line->render_from_to(getComponent<Transform>()->get_position(), m_target_enemy->get_pos(), glm::vec3(1.f));
}

void BaseTower::set_target(Enemy* target)
{
	m_target_enemy = target;
}

glm::vec3 BaseTower::get_pos()
{
	return getComponent<Transform>()->get_position();
}

Enemy* BaseTower::get_target()
{
	return m_target_enemy;
}
