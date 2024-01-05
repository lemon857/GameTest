#include "EngineCore/System/SysFunc.h"

#include <random>

namespace sysfunc
{
	bool is_full(const char* str, int strLen, char symbol)
	{
		for (int i = 0; i < strLen; i++)
		{
			if (str[i] != symbol) return false;
		}
		return true;
	}

	int get_random(int min, int max)
	{		
		return (rand() % (max - min)) + min;
	}
}