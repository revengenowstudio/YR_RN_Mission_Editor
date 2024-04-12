#include "stdafx.h"
#include "../MissionEditor/IniFile.h"

TEST(CIniFileClass, LoadFileTest) {
	auto const iniContent = R"(
[Debug]
;DisplayAllOverlay=Yes ; Doesn´t cripple the overlay list in any way
;EnableTrackLogic=Yes ; Enables Track Logic
;IgnoreSHPImageHeadUnused=Yes ; Use this *carefully* to make SHP graphics of some mods work that incorrectly have the shadow flag set
AllowTunnels=yes
AllowUnidirectionalTunnels=yes
)";

	auto const testIni = "test.ini";
	std::ofstream iniFile(testIni);
	iniFile << iniContent;
	iniFile.flush();
	iniFile.close();

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(testIni)), 0);

	EXPECT_EQ(true, file.GetBool("Debug","AllowTunnels"));
}