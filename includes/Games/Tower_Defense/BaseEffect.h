#pragma once

#include "EngineCore/System/List.h"

class BaseEnemy;
enum class TypeAttack;

class BaseEffect
{
public:
	// set damage per second
	BaseEffect(int damage, double cooldown, double duration);
	~BaseEffect();

	// get damage
	virtual void update(const double delta);

	virtual void modify_enemy() {}
	virtual void unmodify_enemy() {}

	void set_cur_enemy(BaseEnemy* cur_enemy);

	bool is_destroy() { return m_isDestroyed; }

	double get_duration() { return m_duration; }

	double get_live_time() { return m_live_time; }
protected:
	int m_damage;

	double m_cur_time;

	double m_live_time;

	double m_cool_down;

	double m_duration;

	bool m_isDestroyed;
	TypeAttack m_type_attack;
	BaseEnemy* m_cur_enemy;
};
