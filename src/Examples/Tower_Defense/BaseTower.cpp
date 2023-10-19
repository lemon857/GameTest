#include "Games/Tower_Defense/BaseTower.h"

#include "Games/Tower_Defense/Enemy.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/Line.h"

#include <glm/gtc/matrix_transform.hpp>

BaseTower::BaseTower(ObjModel* model, Enemy* target, glm::vec3 pos, double cooldown, RenderEngine::Line* line)
	: m_model(std::move(model))
	, m_target_enemy(std::move(target))
	, m_line(std::move(line))
	, m_cool_down(cooldown * 1000)
	, m_cur_time(0)	
{
	m_model->addComponent<Transform>(pos);
}

BaseTower::~BaseTower()
{
	delete m_model;
}

void BaseTower::update(const double delta)
{
	if (m_target_enemy == nullptr) return;
	glm::vec3 a = m_target_enemy->get_pos() - m_model->getComponent<Transform>()->get_position();
	if (sqrt(a.x * a.x + a.z * a.z) < MIN_DISTANCE_TO_ENEMY)
	{
		if (m_cur_time < m_cool_down) m_cur_time += delta;
		else
		{
			if (m_target_enemy != nullptr) m_target_enemy->damage(5);
			m_cur_time = 0;
		}
		m_line->render_from_to(m_model->getComponent<Transform>()->get_position(), m_target_enemy->get_pos(), glm::vec3(1.f));
	}
	m_model->update(delta);
}

void BaseTower::set_target(Enemy* target)
{
	m_target_enemy = target;
}
