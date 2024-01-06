#include "Games/Tower_Defense/BaseEnemy.h"

#include "Games/Tower_Defense/Castle.h"
#include "Games/Tower_Defense/BaseEffect.h"
#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/System/SysFunc.h"
#include "Games/Tower_Defense/DamageTable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#define HALF_PLANE 30.f

BaseEnemy::BaseEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, const double cooldown, const double velocity, const unsigned int hp, const unsigned int damage,
	std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 color)
	: m_cooldown(cooldown * 1000)
	, m_target_castle(std::move(target))
	, m_cur_time(0)
	, m_velocity(velocity * 0.001)
	, m_model(std::move(model))
	, m_bar(new HealthBar(pMaterial, pos + glm::vec3(0.f, 2.5f, 0.f), 25, 2, hp, glm::vec3(1.f), color))
	, m_bar_effect(new HealthBar(pMaterial, pos + glm::vec3(0.f, 4.5f, 0.f), 25, 2, hp, glm::vec3(1.f), glm::vec3(1.f)))
	, m_isDestroyed(false)
	, m_has_effect(false)
	, m_hp(hp)
	, m_max_hp(hp)
	, m_dmg(damage)
	, m_effect(nullptr)
	, curtarget(0)
	, m_targets(targets)
{
	m_model->addComponent<Transform>(pos, glm::vec3(1.f));
	for (size_t i = 0; i < m_targets.size(); i++)
	{
		m_targets[i].set_pos(m_targets[i].get_pos() + glm::vec3((float)sysfunc::get_random(-25, 25) / 10.f, 0.f, (float)sysfunc::get_random(-25, 25) / 10.f));
	}
}

BaseEnemy::~BaseEnemy()
{
	delete m_bar;
	delete m_bar_effect;
	m_effect = nullptr;
}

void BaseEnemy::update(const double delta)
{
	if (m_isDestroyed) return;
	on_update(delta);
	if (curtarget < m_targets.size())
	{
		glm::vec3 a = m_targets[curtarget].get_pos() - m_model->getComponent<Transform>()->get_position();
		if (sqrt(a.x * a.x + a.z * a.z) > 0.1f)
		{
			//if (m_model->getComponent<Transform>()->get_position().x > HALF_PLANE)
			//{
				m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
			//}
			//else
			//{
			//	m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, -1 * glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
			//}
			m_model->getComponent<Transform>()->add_position(glm::normalize(a)* (float)delta* (float)m_velocity);
			m_bar->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.5f, 0.f));
			m_bar_effect->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 4.5f, 0.f));
		}
		else
		{
			curtarget++;
		}
	}
	else if (curtarget == m_targets.size())
	{
		glm::vec3 a = m_target_castle->get_pos() - m_model->getComponent<Transform>()->get_position();
		if (sqrt(a.x * a.x + a.z * a.z) > MIN_DISTANCE_TO_CASTLE)
		{
			//if (m_model->getComponent<Transform>()->get_position().x > HALF_PLANE)
			//{
				m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
			//}
			//else
			//{
			//	m_model->getComponent<Transform>()->set_rotation(glm::vec3(0.f, -1 * glm::degrees(glm::angle(glm::normalize(a), glm::vec3(0.f, 0.f, -1.f))), 0.f));
			//}
			m_model->getComponent<Transform>()->add_position(glm::normalize(a) * (float)delta * (float)m_velocity);
			m_bar->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 2.5f, 0.f));
			m_bar_effect->set_pos(m_model->getComponent<Transform>()->get_position() + glm::vec3(0.f, 4.5f, 0.f));
		}
		else
		{
			if (m_cur_time < m_cooldown) m_cur_time += delta;
			else
			{
				m_target_castle->damage(m_dmg);
				m_cur_time = 0;
			}
		}
	}

	if (m_effect != nullptr)
	{
		if (m_effect->is_destroy())
		{
			m_effect = nullptr;
			m_has_effect = false;
		}
		else m_effect->update(delta);
	}
}

void BaseEnemy::render()
{
	if (m_isDestroyed) return;
	m_model->update(0);
	m_bar->update();
	if (m_effect != nullptr)
	{
		m_bar_effect->set_value(m_effect->get_live_time());
		m_bar_effect->update();
	}
}

void BaseEnemy::damage(const double damage_hp)
{
	if (m_hp - damage_hp <= m_max_hp) m_hp -= damage_hp;
	if (m_hp <= 0)
	{
		m_isDestroyed = true;
		return;
	}
	m_bar->set_value(m_hp);
}

glm::vec3 BaseEnemy::get_pos()
{
	return m_model->getComponent<Transform>()->get_position();
}

void BaseEnemy::set_effect(std::unique_ptr<BaseEffect> effect)
{
	if (m_effect != nullptr) m_effect = nullptr;
	m_effect = std::move(effect);
	m_bar_effect->set_max_value(m_effect->get_duration());
	m_effect->set_cur_enemy(this);
	m_has_effect = true;
}

void BaseEnemy::set_color_effect(glm::vec3 colorFore, glm::vec3 colorBack)
{
	m_bar_effect->set_fore_color(colorFore);
	m_bar_effect->set_back_color(colorBack);
}

std::string BaseEnemy::get_type_str(TypeArmor type)
{
	switch (type)
	{
	case TypeArmor::Null:
		return "null";
	case TypeArmor::Light:
		return "light";
	case TypeArmor::Heavy:
		return "heavy";
	case TypeArmor::Magic:
		return "magic";
	case TypeArmor::Chaotic:
		return "chaotic";
	}
	return "";
}
