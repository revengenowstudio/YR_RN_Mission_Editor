#pragma once

#include <afxcview.h>
#include "INIMeta.h"
#include "variables.h"

extern CIniFile rules;

class IniMegaFile
{
	friend class IniFileGroup;
public:
	static IniFileGroup GetRules();

	static bool IsNullOrEmpty(const  CString& value) { return isNullOrEmpty(value); }

private:
	static bool isNullOrEmpty(const CString& value);
};


IniFileGroup IniMegaFile::GetRules()
{
	IniFileGroup m_group;
	m_group.Append(rules);
	m_group.Append(Map->GetIniFile());
	return m_group;
}

bool IniMegaFile::isNullOrEmpty(const CString& value)
{
	return !value.GetLength() || value == "none" || value == "<none>";
}