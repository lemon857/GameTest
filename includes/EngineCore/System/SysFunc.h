#pragma once

#include <string>

namespace sysfunc
{
	template<class _Ty>
	void type_to_char(_Ty* val, char* buff, unsigned int offset)
	{
		char* buf = (char*)val;

		for (int i = 0; i < sizeof(_Ty); i++)
		{
			buff[i + offset] = buf[i];
		}
	}

	template<class _Ty>
	void char_to_type(_Ty* val, char* buff, unsigned int offset)
	{
		*val = *(_Ty*)(&buff[offset]);
	}
	
	bool start_with(std::string& line, const char* text);

	// if str full contains symbol return true
	bool is_full(const char* str, int strLen, char symbol);
		
	double conv_range(double value, double val_min, double val_max, double new_min, double new_max);

	int get_random(int min, int max);
	float get_random(float min, float max);
	double get_random(double min, double max);

	void swap(void** a, void** b);
	int partition(void** arr, int low, int high);
	void quickSort(void** arr, int low, int high);

	std::string ctostr(std::wstring str);
	std::wstring ctowstr(std::string str);
}