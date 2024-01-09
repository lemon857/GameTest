#include "EngineCore/System/SysFunc.h"

#include <random>
#include <locale>
#include <codecvt>

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
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}
	float get_random(float min, float max)
	{
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(generator);
	}
	double get_random(double min, double max)
	{
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}
	void swap(void** a, void** b)
	{
		void* t = *a;
		*a = *b;
		*b = t;
	}
	int partition(void** arr, int low, int high)
	{

		void* pivot = arr[high];

		int i = (low - 1);

		for (int j = low; j < high; j++) {
			if (arr[j] <= pivot) {

				i++;

				swap(&arr[i], &arr[j]);
			}
		}

		swap(&arr[i + 1], &arr[high]);

		return (i + 1);
	}
	void quickSort(void** arr, int low, int high)
	{
		if (low < high) {

			int pi = partition(arr, low, high);

			quickSort(arr, low, pi - 1);

			quickSort(arr, pi + 1, high);
		}
	}
	std::string ctostr(std::wstring str)
	{
		return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(str);
	}
	std::wstring ctowstr(std::string str)
	{
		return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(str);
	}
}