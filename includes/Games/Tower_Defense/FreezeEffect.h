#pragma once

#include "Games/Tower_Defense/BaseEffect.h"

class FreezeEffect : public BaseEffect
{
public:
	FreezeEffect(double time_freeze);
	~FreezeEffect() = default;

	void modify_enemy() override;
	void unmodify_enemy() override;

private:
	double m_last_vel;
};