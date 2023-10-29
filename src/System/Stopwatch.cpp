#include "EngineCore/System/Stopwatch.h"

Stopwatch::Stopwatch()
	: m_timeStart(std::chrono::high_resolution_clock::now())
{
}

void Stopwatch::start()
{
	m_timeStart = std::chrono::high_resolution_clock::now();
}

double Stopwatch::stop()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration<double, std::milli>(currentTime - m_timeStart).count();
	return duration;
}
