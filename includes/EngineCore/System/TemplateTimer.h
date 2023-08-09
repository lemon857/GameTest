#pragma once

#include "Timer.h"

template <class _Ty>
class TemplateTimer : public Timer
{
public:
	TemplateTimer(_Ty callbackArg)
		: Timer()
		, m_callbackArg(callbackArg)
	{
	};
	void setTemplateCallback(std::function<void(_Ty)> callback)
	{
		m_templateCallback = callback;
	};
	void update(const double delta) override
	{
		if (!m_isRunning) return;
		m_timeLeft -= delta;
		if (m_timeLeft <= 0)
		{
			m_isRunning = false;
			m_templateCallback(m_callbackArg);
		}
	};
private:
	std::function<void(_Ty)> m_templateCallback;
	_Ty m_callbackArg;
};