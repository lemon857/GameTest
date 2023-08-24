#pragma once

#include <chrono>

class Stopwatch
{
public:
	Stopwatch();

	void start();
	// Вернёт разницу во времени между вызовом функций старт и стоп
	double stop();

private:
	std::chrono::steady_clock::time_point m_timeStart;
};
