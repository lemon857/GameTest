#include "Games/Tower_Defense/EnemyMagician.h"

#include "Games/Tower_Defense/DamageTable.h"
#include "Games/Tower_Defense/HealthEffect.h"
#include "Games/Tower_Defense/HealthBar.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/Circle.h"

MagicianEnemy::MagicianEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos,
	std::shared_ptr<RenderEngine::Material> pMaterial, linked_list<BaseEnemy*>* list)
	: BaseEnemy(std::move(model), std::move(target), targets, pos, P_MAGICIAN_COOLDOWN, P_MAGICIAN_VELOCITY, P_MAGICIAN_HP, P_MAGICIAN_DAMAGE, pMaterial,
		glm::vec3(0.7f, 0.3f, 0.8f))
	, m_isActiveCircle(false)
	, m_cur_time_cir(0)
	, m_dur_time_cir(2000)
	, m_cur_time_treat(0)
	, m_dur_time_treat(10000)
	, m_list(std::move(list))
{
	m_type_armor = TypeArmor::Magic;
	m_reward = 6;
	m_model->getComponent<Transform>()->set_scale(glm::vec3(0.5f));
	m_health = new RenderEngine::Circle(pMaterial, pos - glm::vec3(0.f, 100.f, 0.f), glm::vec3(1.f), glm::vec3(0.f),
		glm::vec4(0.f, 1.f, 0.3f, 0.7f), P_MAGICIAN_RADIUS_TREAT, 90, 3);
}

void MagicianEnemy::render()
{
	if (m_isDestroyed) return;
	m_model->update(0);
	m_bar->update();
	if (m_effect != nullptr)
	{
		m_bar_effect->set_value(m_effect->get_live_time());
		m_bar_effect->update();
	}
	m_health->render();
}

void MagicianEnemy::on_update(const double delta)
{
	if (m_cur_time_treat < m_dur_time_treat) m_cur_time_treat += delta;
	else
	{
		glm::vec3 pos = get_pos();
		m_health->set_pos(pos);
		m_isActiveCircle = true;
		for (size_t i = 0; i < m_list->size(); i++)
		{
			if (m_list->at(i) == nullptr) continue;
			glm::vec3 p = m_list->at(i)->get_pos();
			if (p == pos) continue;
			if (((p.x - pos.x) * (p.x - pos.x)) + ((p.z - pos.z) * (p.z - pos.z)) < P_MAGICIAN_RADIUS_TREAT * P_MAGICIAN_RADIUS_TREAT)
			{
				if (!m_list->at(i)->has_effect())
				{
					m_list->at(i)->set_effect(std::make_unique<HealthEffect>(4));
				}
			}
		}
		m_cur_time_treat = 0;
	}
	if (!m_isActiveCircle) return;
	if (m_cur_time_cir < m_dur_time_cir) m_cur_time_cir += delta;
	else
	{
		m_health->set_pos(get_pos() - glm::vec3(0.f, 100.f, 0.f));
		m_isActiveCircle = false;
		m_cur_time_cir = 0;
	}
}
