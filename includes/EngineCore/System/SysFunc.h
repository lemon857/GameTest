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
}