#include "stdafx.h"
#include "../MissionEditor/INIMeta.h"

#if 1
TEST(IniFileGroup, ValidIniWithValidContentTest) {
	CIniFile iniFile1;
	CIniFile iniFile2;

	const CString referenceList[] = {
		CString("GANCST"),
		CString("GAPOWR"),
		CString("NACNST"),
		CString("NAPOWR"),
	};

	iniFile1.SetString("BuildingTypes", "0", "GANCST");
	iniFile1.SetString("BuildingTypes", "1", "GAPOWR");
	iniFile1.SetString("GACNST", "Strength", "1000");
	iniFile1.SetString("E2", "Strength", "100");

	iniFile2.SetString("BuildingTypes", "2", "NACNST");
	iniFile2.SetString("BuildingTypes", "3", "NAPOWR");
	iniFile2.SetString("GACNST", "Strength", "2000");
	iniFile2.SetString("NEWIFV", "Cost", "1000");

	IniFileGroup group;
	group.Append(iniFile1);
	group.Append(iniFile2);

	auto const bldTypes = group.GetSection("BuildingTypes");
	auto idx = 0;
	//for (auto const& [key, val] : bldTypes) {
	for (auto it = bldTypes.begin(); it != bldTypes.end(); ++it) {
		auto const& [key, val] = *it;
		EXPECT_EQ(val, referenceList[idx++]);
	}

	EXPECT_EQ(group.GetString("GACNST", "Strength"), "2000");
}
#endif

TEST(IniFileGroup, EmptyIniContentTest) {
	CIniFile iniFile1;
	CIniFile iniFile2;
	IniFileGroup group;

	group.Append(iniFile1);
	group.Append(iniFile2);

	auto const bldTypes = group.GetSection("BuildingTypes");
	auto contentCount = 0;
	for (auto it = bldTypes.begin(); it != bldTypes.end(); ++it) {
		auto const& [key, val] = *it;
		cout << key << "=" << val << endl;
		contentCount++;
	}

	EXPECT_EQ(contentCount, 0);

	iniFile1.SetString("BuildingTypes", "0", "GANCST");

	contentCount = 0;
	for (auto it = bldTypes.begin(); it != bldTypes.end(); ++it) {
		auto const& [key, val] = *it;
		cout << key << "=" << val << endl;
		contentCount++;
	}

	EXPECT_EQ(contentCount, 1);
}