/*
	FinalSun/FinalAlert 2 Mission Editor

	Copyright (C) 1999-2024 Electronic Arts, Inc.
	Authored by Matthias Wagner

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// IniFile.h: Schnittstelle für die Klasse CIniFile.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__96455620_6528_11D3_99E0_DB2A1EF71411__INCLUDED_)
#define AFX_INIFILE_H__96455620_6528_11D3_99E0_DB2A1EF71411__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <map>
#include <CString>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <ios>
#include "IniHelper.h"


using namespace std;

class SortDummy
{
public:
	bool operator() (const CString&, const CString&) const;
};



class CIniFileSection
{
public:

	static const CString EmptyValue;

	CIniFileSection();
	virtual ~CIniFileSection();

	[[deprecated("instead use GetString or TryGetString")]]
	CString& AccessValueByName(const CString& name);

	auto const& Nth(size_t index) const {
		ASSERT(index < value_pairs.size());
		return this->value_pairs[index];
	}

	const CString* TryGetString(const CString& key) const {
		auto const it = value_pos.find(key);
		if (it != value_pos.end()) {
			return &this->value_pairs[it->second].second;
		}
		return nullptr;
	}

	const CString& GetString(const CString& key) const {
		if (auto const ret = TryGetString(key)) {
			return *ret;
		}
		return EmptyValue;
	}
	CString GetStringOr(const CString& key, const CString& defaultValue) const {
		auto const it = value_pos.find(key);
		if (it != value_pos.end()) {
			return this->value_pairs[it->second].second;
		}
		return defaultValue;
	}

	int GetInteger(const CString& key, int def = 0)const {
		return INIHelper::StringToInteger(this->GetString(key), def);
	}

	size_t Size() const { return value_pos.size(); }

	bool Exists(const CString& key) const {
		auto const it = value_pos.find(key);
		return it != value_pos.end();
	}

	void Assign(const CString& key, const CString& value) {
		return this->Assign(key, CString(value));
	}

	void Assign(const CString& key, CString&& value) {
		auto const it = value_pos.find(key);
		// new, never had one
		if (it == value_pos.end()) {
			this->value_pairs.push_back({ key, std::move(value) });
			value_pos[key] = value_pairs.size();
			return;
		}
		value_pairs[it->second].second = std::move(value);
	}

	bool HasValue(const CString& val) const {
		return this->FindValue(val) >= 0;
	}

	void RemoveAt(size_t idx) {
		ASSERT(idx < value_pairs.size());
		for (auto affectedIdx = idx + 1; affectedIdx < value_pairs.size(); ++affectedIdx) {
			auto const& kvPair = value_pairs[affectedIdx];
			auto const it = value_pos.find(kvPair.first);
			ASSERT(it != value_pos.end());
			it->second--;
		}
		auto const itErased = value_pairs.erase(value_pairs.begin() + idx);
		ASSERT(value_pos.erase(itErased->first), 1);
	}

	auto begin() const noexcept
	{
		return value_pairs.begin();
	}

	auto end() const noexcept
	{
		return value_pairs.end();
	}

	[[deprecated("instead use iterators or for_each")]]
	int GetValueOrigPos(int index) const noexcept;

	[[deprecated("instead use iterators or for_each")]]
	int FindName(CString sval) const noexcept;

	[[deprecated("instead use iterators or for_each")]]
	int FindValue(CString sval) const noexcept;

	[[deprecated("instead use iterators or for_each")]]
	const CString* GetValueName(std::size_t index) const noexcept;

private:
	map<CString, int, SortDummy> value_pos{};
	vector<std::pair<CString, CString>> value_pairs{};// sequenced
	mutable bool isRegistry{false};
};

class CIniFile
{
	static const CIniFileSection EmptySection;

public:
	const CString* GetSectionName(std::size_t Index) const noexcept;
	const CIniFileSection* TryGetSection(std::size_t index) const;
	CIniFileSection* TryGetSection(std::size_t index);

	const CIniFileSection* TryGetSection(const CString& section) const
	{
		auto pMutThis = const_cast<std::remove_cv_t<std::remove_pointer_t<decltype(this)>>*>(this);
		return pMutThis->TryGetSection(section);
	}

	CIniFileSection* TryGetSection(const CString& section)
	{
		auto it = sections.find(section);
		if (it != sections.end()) {
			return &it->second;

		}
		return nullptr;
	}

	const CIniFileSection& GetSection(const CString& section) const {
		auto const it = sections.find(section);
		if (it != sections.end()) {
			return it->second;
		}
		return EmptySection;
	}

	CString GetValueByName(const CString& sectionName, const CString& valueName, const CString& defaultValue) const;
	void Clear();
	WORD InsertFile(const CString& filename, const char* Section, BOOL bNoSpaces = FALSE);
	WORD InsertFile(const std::string& filename, const char* Section, BOOL bNoSpaces = FALSE);
	BOOL SaveFile(const CString& Filename) const;
	BOOL SaveFile(const std::string& Filename) const;
	WORD LoadFile(const CString& filename, BOOL bNoSpaces = FALSE);
	WORD LoadFile(const std::string& filename, BOOL bNoSpaces = FALSE);

	const CString& GetString(const CString& section, const CString& key) const {
		auto const it = sections.find(section);
		if (it != sections.end()) {
			return it->second.GetString(key);
		}
		return CIniFileSection::EmptyValue;
	}
	const bool GetBool(const CString& section, const CString& key, bool def = false) const {
		auto const& str = this->GetString(section, key);
		return INIHelper::StingToBool(str, def);
	}

	void Assign(const CString& section, const CString& key, CString&& value) {
		auto const it = sections.find(section);
		if (it != sections.end()) {
			it->second.Assign(key, value);
			return;
		}
		auto&& newSec = CIniFileSection{};
		newSec.Assign(key, value);
		ASSERT(sections.insert({ key, std::move(newSec) }).second == true);
	}

	void Assign(const CString& section, const CString& key, const CString& value) {
		return this->Assign(section, key, CString(value));
	}

	auto begin() noexcept
	{
		return sections.begin();
	}

	auto begin() const noexcept
	{
		return sections.begin();
	}

	auto end() noexcept
	{
		return sections.end();
	}

	auto end() const noexcept
	{
		return sections.end();
	}

	CIniFile();
	virtual ~CIniFile();

private:
	std::string m_filename;
	map<CString, CIniFileSection> sections;
};

#endif // !defined(AFX_INIFILE_H__96455620_6528_11D3_99E0_DB2A1EF71411__INCLUDED_)
