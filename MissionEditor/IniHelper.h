#pragma once
#include <CString>
#include <ctype.h>

class INIHelper
{
public:
	static bool StringToBool(const CString& str, bool def)
	{
		switch (toupper(static_cast<unsigned char>(*str))) {
		case '1':
		case 'T':
		case 'Y':
			return true;
		case '0':
		case 'F':
		case 'N':
			return false;
		default:
			return def;
		}
	}
	static int StringToInteger(const  CString& str, int def)
	{
		int ret = 0;
		if (sscanf_s(str, "%d", &ret) == 1) {
			return ret;
		}
		return def;
	}

	template<typename T>
	static CString ToString(const T& origin);// { static_assert(false, "T must have specialized implementations!"); }

	template<>
	static CString ToString<bool>(const bool& origin) 
	{
		static CString result[] = {  "no", "yes" };
		return result[origin];
	}
};