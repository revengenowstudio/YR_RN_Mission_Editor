#include "StdAfx.h"
#include "INIMeta.h"
#include <set>

void IniFileGroup::Append(const CIniFile& INI)
{
	m_group.push_back(&INI);
}


const CIniFile* IniFileGroup::Nth(int idx) const
{
	return m_group.at(idx);
}

const CString& IniFileGroup::GetString(const CString & section, const CString & key) const
{
	for (auto it = m_group.rbegin(); it != m_group.rend(); ++it) {
		auto const& got = (*it)->GetString(section, key);
		if (!got.IsEmpty()) {
			return got;
		}
	}
	return CIniFileSection::EmptyValue;
}

CString IniFileGroup::GetStringOr(const CString & section, const CString & key, const CString& def) const
{
	auto const& got = this->GetString(section, key);
	if (!got.IsEmpty()) {
		return got;
	}
	return def;
}

IniSectionGroup IniFileGroup::GetSection(const CString& section) const
{
	return IniSectionGroup(*this, section);
}