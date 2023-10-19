#include "Games/Tower_Defense/Enemy.h"

#include "Games/Tower_Defense/Castle.h"
#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

Enemy::Enemy(ObjModel* model, Castle* target, glm::vec3 pos, double cooldown, double velocity, const unsigned int hp, std::shared_ptr<RenderEngine::Material> pMaterial)
	: m_cool_down(cooldown * 1000)
	, m_target_castle(std::move(target))
	, m_cur_time(0)
	, m_velocity(velocity)
	, m_model(std::move(model))
	, m_bar(new HealthBar(pMaterial, pos + glm::vec3(0.f, 2.5f, 0.f), 25, 2, hp, glm::vec3(1.f), glm::vec3(1.f, 0.f, 0.f)))
	, m_isDestroyed(false)
{
	m_model->addComponent<Transform>(pos);
}

Enemy::~Enemy()
{
}

void Enemy::update(const double delta)
{
	if (m_isDestroyed) return;
	glm::vec3 a = m_target_castle->get_pos() - m_model->getComponent<Transform>()->get_position();
	if (sqrt(a.x * a.x + a.z * a.z) > MIN_DISTANCE)
	{
		m_model->getComponent<Transform>()->add_position(glm::normalize(a) * (float)delta * (float)m_velocity);
		m_bar->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.5f, 0.f));
	}
	else
	{
		if (m_cur_time < m_cool_down) m_cur_time += delta;
		else
		{
			m_target_castle->damage(5);
			m_cur_time = 0;
		}
	}
	m_model->update(delta);
	m_bar->update();
}

void Enemy::damage(const unsigned int damage_hp)
{
	m_hp -= damage_hp;
	m_bar->set_value(m_hp);
	if (m_hp <= 0)
	{
		dead();
	}
}

void Enemy::dead()
{
	m_isDestroyed = true;
}
