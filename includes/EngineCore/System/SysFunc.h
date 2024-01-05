#pragma once

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

	// if str full contains symbol return true
	bool is_full(const char* str, int strLen, char symbol);
		
	int get_random(int min, int max);	
}