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
}