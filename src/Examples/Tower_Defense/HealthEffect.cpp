#include "Games/Tower_Defense/HealthEffect.h"

#include "Games/Tower_Defense/BaseEnemy.h"

HealthEffect::HealthEffect(double health_add)
	: BaseEffect(0, 0, 0)
	, m_health(health_add)
{
}

void HealthEffect::modify_enemy()
{
	m_cur_enemy->damage(-m_health);
}
