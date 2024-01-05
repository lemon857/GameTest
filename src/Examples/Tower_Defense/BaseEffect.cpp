#include "Games/Tower_Defense/BaseEffect.h"

#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/DamageList.h"

BaseEffect::BaseEffect(unsigned int damage, double cooldown, double duration)
	: m_damage(damage)
	, m_cool_down(cooldown * 1000)
	, m_cur_time(0)
	, m_live_time(0)
	, m_duration(duration * 1000)
	, m_cur_enemy(nullptr)
	, m_isDestroyed(false)
{
}

BaseEffect::~BaseEffect()
{
}

void BaseEffect::update(const double delta)
{
	if (m_cur_time < m_cool_down) m_cur_time += delta;
	else
	{
		m_cur_enemy->damage(m_damage * damageList[(int)m_type_attack][(int)m_cur_enemy->get_type()]);
		m_cur_time = 0;
	}
	if (m_live_time < m_duration) m_live_time += delta;
	else
	{
		unmodify_enemy();
		m_isDestroyed = true;
	}
}

void BaseEffect::set_cur_enemy(BaseEnemy* cur_enemy)
{
	m_cur_enemy = std::move(cur_enemy);
	modify_enemy();
}
