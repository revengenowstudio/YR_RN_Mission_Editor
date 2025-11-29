#include "stdafx.h"
#include "TriggerDef.h"
#include "Helpers.h"
#include "IniFile.h"

TEST(TriggerDefinitionTest, LoadIni)
{
    std::stringstream testIniContent;
    testIniContent << 
R"(
[ParamTypes]
-1=Unused,0,1;not listed in FA2
0=Nothing,0
1=Unknown,0
2=House,1

[ActionsRA2]
0=-Nothing-,0,0,0,0,0,0,0,0,0,This is an empty action. It means doing nothing.,0,1,0
1=Claim Winner...,0,2,0,0,0,0,0,0,0,The winner must be a specific side, and the game will end immediately. For example, in %1, the players on a specific side are defined. In a multiplayer task, this action will lead to the failure of all players.,0,1,1
2=Claim Loser...,0,2,0,0,0,0,0,0,0,The loser must be a specific side, and the game will end immediately. When a specific side is designated as the loser, the game ends right away. For example, in %1, the players on a specific side are defined. If a non-player country is set as the loser, it can result in a mission victory; a typical example is a modified mod that changes the order of countries. In a multiplayer campaign, this action will lead to the failure of all players.,0,1,2
)";

    CIniFile ini;
    EXPECT_EQ(ini.InsertStream(testIniContent), 0);

    auto& mgr = TriggerDefinitionManager::Instance();
    mgr.LoadFrom(ini, std::cerr);

    EXPECT_EQ(mgr.Actions().size(), 3);
    EXPECT_EQ(mgr.Actions().at(0).description, "This is an empty action. It means doing nothing.");

    EXPECT_EQ(mgr.Params().size(), 4);
    EXPECT_EQ(mgr.Params().at(-1).paramName, "Unused");
    EXPECT_EQ(mgr.Params().at(0).paramName, "Nothing");
    EXPECT_EQ(mgr.Params().at(1).paramName, "Unknown");
    EXPECT_EQ(mgr.Params().at(2).paramName, "House");
    EXPECT_EQ(mgr.Params().at(2).listType, 1);
}