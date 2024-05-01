#pragma once

#include <afx.h>
#include <vector>

namespace utilities {
	static std::vector<CString> split_string(const CString& pSource, TCHAR cSplit = ',')
	{
		std::vector<CString> ret;
		CString tmp = pSource;
		int pos = 0;

		while ((pos = tmp.Find(cSplit)) != -1) {
			ret.push_back(tmp.Left(pos));
			tmp = tmp.Mid(pos + 1);
		}

		if (!tmp.IsEmpty()) {
			ret.push_back(tmp);
		}

		return ret;
	}
}

