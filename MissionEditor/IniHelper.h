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
	static double StringToDouble(const  CString& str, double def)
	{
		double ret = 0;
		if (sscanf_s(str, "%lf", &ret) == 1) {
			if (strchr(str, '%')) {
				ret *= 0.01;
			}
			return ret;
		}
		return def;
	}
	static float StringToFloat(const  CString& str, float def)
	{
		return static_cast<float>(StringToDouble(str, def));
	}

	template<typename T>
	static CString ToString(const T& origin);// { static_assert(false, "T must have specialized implementations!"); }

	template<>
	static CString ToString<bool>(const bool& origin)
	{
		static CString result[] = { "no", "yes" };
		return result[origin];
	}

	template<>
	static CString ToString<int>(const int& origin)
	{
		char buffer[0x100];
		_itoa_s(origin, buffer, 10);
		buffer[sizeof buffer - 1] = '\0';
		return buffer;
	}
};