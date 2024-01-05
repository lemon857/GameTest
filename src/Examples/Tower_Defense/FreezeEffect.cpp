#include "Games/Tower_Defense/FreezeEffect.h"

#include "Games/Tower_Defense/BaseEnemy.h"

FreezeEffect::FreezeEffect()
	: BaseEffect(1, 3, 15)
	, m_last_vel(0)
{
}

void FreezeEffect::modify_enemy()
{
	m_last_vel = m_cur_enemy->get_vel();
	m_cur_enemy->set_vel(m_last_vel - m_last_vel * 0.5);
	m_cur_enemy->set_color_effect(glm::vec3(1.f), glm::vec3(0.f, 0.5f, 0.9f));
}

void FreezeEffect::unmodify_enemy()
{
	m_cur_enemy->set_vel(m_last_vel);
}
