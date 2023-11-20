#pragma once

#include "EngineCore/System/List.h"

class BaseEnemy;

class BaseEffect
{
public:
	// set damage per second
	BaseEffect(unsigned int damage, double cooldown, double duration);
	~BaseEffect();

	// get damage
	void update(const double delta);

	void set_cur_enemy(BaseEnemy* cur_enemy);

	bool is_destroy() { return m_isDestroyed; }

protected:
	unsigned int m_damage;

	double m_cur_time;

	double m_live_time;

	double m_cool_down;

	double m_duration;

	bool m_isDestroyed;

	BaseEnemy* m_cur_enemy;
};
