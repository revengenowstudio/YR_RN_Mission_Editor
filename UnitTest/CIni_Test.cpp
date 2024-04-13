#include "stdafx.h"
#include "../MissionEditor/IniFile.h"

class IniTestHelper
{
	std::string m_fileName;

	void writeDownContent(const char* pContent) {
		std::ofstream iniFile(m_fileName.c_str());
		ASSERT(iniFile.is_open() == true);
		iniFile << pContent;
		iniFile.flush();
		iniFile.close();
	}

	public :
		IniTestHelper(std::string&& name, const char* pContent) :
			m_fileName(std::move(name))
		{
			ASSERT(!m_fileName.empty());
			ASSERT(pContent != nullptr);
			writeDownContent(pContent);
		}
		~IniTestHelper() {
			remove(m_fileName.c_str());
		}

};

TEST(CIniFileClass, LoadFileTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[Debug]
;DisplayAllOverlay=Yes ; Doesn´t cripple the overlay list in any way
;EnableTrackLogic=Yes ; Enables Track Logic
;IgnoreSHPImageHeadUnused=Yes ; Use this *carefully* to make SHP graphics of some mods work that incorrectly have the shadow flag set
AllowTunnels=yes
AllowUnidirectionalTunnels=yes

[BuildingVoxelTurretsRA2OLD]
GTGCANX=00
GTGCANY=44;-6
)");


	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);

	EXPECT_NE(file.Size(), 0);
	EXPECT_EQ(true, file.GetBool("Debug", "AllowTunnels"));
	EXPECT_EQ(false, file.GetBool("Debug", "DisplayAllOverlay"));
	EXPECT_EQ("00", file.GetString("BuildingVoxelTurretsRA2OLD", "GTGCANX"));
}


TEST(CIniFileClass, IniSequenceTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[SlopeSetPiecesDirections]
Count=10
0=Right_1
1=Left_1
2=Top_1
3=Bottom_1
4=Right_2
5=Left_2
6=Left_2
7=Bottom_2
8=Top_2
9=Top_2
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);
	EXPECT_EQ(11, file["SlopeSetPiecesDirections"].Size());
	EXPECT_EQ("10", file["SlopeSetPiecesDirections"].Nth(0).second);

}

TEST(CIniFileClass, IniSetValueTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[Debug]
;DisplayAllOverlay=Yes ; Doesn´t cripple the overlay list in any way
;EnableTrackLogic=Yes ; Enables Track Logic
;IgnoreSHPImageHeadUnused=Yes ; Use this *carefully* to make SHP graphics of some mods work that incorrectly have the shadow flag set
AllowTunnels=yes
AllowUnidirectionalTunnels=yes
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);
	// Test value not exists
	EXPECT_EQ(false, file.GetBool("Debug", "DisplayAllOverlay"));
	file.SetBool("Debug", "DisplayAllOverlay", true);
	EXPECT_EQ(true, file.GetBool("Debug", "DisplayAllOverlay"));
	// Test existed value and override
	EXPECT_EQ(true, file.GetBool("Debug", "AllowTunnels"));
	file.SetBool("Debug", "AllowTunnels", false);
	EXPECT_EQ(false, file.GetBool("Debug", "AllowTunnels"));
	// Test section not exists
	EXPECT_EQ("", file.GetString("SlopeSetPiecesDirections", "0"));
	file.SetString("SlopeSetPiecesDirections", "0", "Right_1");
	EXPECT_EQ("Right_1", file.GetString("SlopeSetPiecesDirections", "0"));
}

TEST(CIniFileClass, IniSetSectionTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[SlopeSetPiecesDirections]
Count=10
0=Right_1
1=Left_1
2=Top_1
3=Bottom_1
4=Right_2
5=Left_2
6=Left_2
7=Bottom_2
8=Top_2
9=Top_2
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);

	CIniFile anotherIni;
	anotherIni.AddSection("Debug");
	auto pDebugSecAnother = anotherIni.TryGetSection("Debug");
	ASSERT_NE(pDebugSecAnother, nullptr);
	pDebugSecAnother->SetBool("DisplayAllOverlay", false);
	pDebugSecAnother->SetBool("AllowTunnels", true);


	file.SetSection("Debug", anotherIni["Debug"]);
	EXPECT_EQ(true, file.GetBool("Debug", "AllowTunnels"));
	EXPECT_EQ(false, file.GetBool("Debug", "DisplayAllOverlay"));
}

TEST(CIniFileClass, IniAddSectionTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[SlopeSetPiecesDirections]
Count=10
0=Right_1
1=Left_1
2=Top_1
3=Bottom_1
4=Right_2
5=Left_2
6=Left_2
7=Bottom_2
8=Top_2
9=Top_2
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);
	file.AddSection("Debug");
	auto pDebugSec = file.TryGetSection("Debug");
	ASSERT_NE(pDebugSec, nullptr);
	pDebugSec->SetBool("DisplayAllOverlay", false);
	pDebugSec->SetBool("AllowTunnels", true);
	EXPECT_EQ(true, file.GetBool("Debug", "AllowTunnels"));
	EXPECT_EQ(false, file.GetBool("Debug", "DisplayAllOverlay"));
}

TEST(CIniFileClass, IniDeleteSectionTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[LUNARLimits]
TreeMax=999
TreeMin=999

[URBANLimits]
TreeMax=999
;TreeMax=27

[TEMPERATELimits]
TreeMax=999
;TreeMax=27

[SNOWLimits]
TreeMax=999
;TreeMax=27

[NEWURBANLimits]
TreeMax=999
;TreeMax=27

[DESERTLimits]
TreeMin=30
TreeMax=999
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);

	EXPECT_EQ(999, file.GetInteger("SNOWLimits", "TreeMax"));
	EXPECT_EQ(999, file.GetInteger("LUNARLimits", "TreeMin"));

	EXPECT_EQ(6, file.Size());
	file.DeleteSection("SNOWLimits");

	EXPECT_EQ(0, file.GetInteger("SNOWLimits", "TreeMax"));
	EXPECT_EQ(nullptr, file.TryGetSection("SNOWLimits"));
	EXPECT_EQ(5, file.Size());
}

TEST(CIniFileClass, IniDeleteValueTest) {
	auto const fileName = "test.ini";
	IniTestHelper helper(fileName, R"(
[LUNARLimits]
TreeMax=999
TreeMin=999

[NewUrbanInfo]
Morphable2=114
Ramps2=117
Cliffs2=110
CliffsWater2=112

; tileset ini overwritings
; only used by FinalAlert
[IgnoreSetTEMPERATE]
0=77
1=78
2=79
)");

	CIniFile file;
	ASSERT_EQ(file.LoadFile(std::string(fileName)), 0);

	EXPECT_EQ(114, file.GetInteger("NewUrbanInfo", "Morphable2"));
	file.RemoveValueByKey("NewUrbanInfo", "Morphable2");

	EXPECT_EQ(0, file.GetInteger("NewUrbanInfo", "Morphable2"));
	EXPECT_EQ(false, file["NewUrbanInfo"].Exists("Morphable2"));

}