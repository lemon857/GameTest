#pragma once

#include <chrono>

class Stopwatch
{
public:
	Stopwatch();

	void start();
	// Âåðížò ðàçíèöó âî âðåìåíè ìåæäó âûçîâîì ôóíêöèé ñòàðò è ñòîï
	double stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_timeStart;
};
