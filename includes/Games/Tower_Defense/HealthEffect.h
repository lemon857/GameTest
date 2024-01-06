#pragma once

#include "Games/Tower_Defense/BaseEffect.h"

class HealthEffect : public BaseEffect
{
public:
	HealthEffect(double health_add);
	~HealthEffect() = default;

	void modify_enemy() override;

	void update(const double delta) override {};

private:
	double m_health;
};