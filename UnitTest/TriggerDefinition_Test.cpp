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

[EventsRA2]
0=-No Event-,0,0,0,0,This is an empty event.,0,1,0
1=Enter...,0,2,0,0,Ground unit enters,0,1,1
2=Discover starts(Unused),0,0,0,1,When a spy enters,0,0,2
3=Stolen by(Unused),0,2,0,1,When money stolen by a thief,0,0,3
8=Anything,0,0,0,0,When use it alone%1 take effect immediately,0,1,8
9=Destroyed%1 Units%1 All...,0,2,0,0,When all techno units get destroyed,0,1,9

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
    EXPECT_EQ(mgr.Actions().at(1).paramTypes[0], 0);
    EXPECT_EQ(mgr.Actions().at(1).paramTypes[1], 2);

    EXPECT_EQ(mgr.Events().size(), 6);
    EXPECT_EQ(mgr.Events().at(0).description, "This is an empty event.");
    EXPECT_EQ(mgr.Events().at(2).obsolete, true);
    EXPECT_EQ(mgr.Events().at(8).description, "When use it alone, take effect immediately");

    EXPECT_EQ(mgr.Params().size(), 4);
    EXPECT_EQ(mgr.Params().at(-1).paramName, "Unused");
    EXPECT_EQ(mgr.Params().at(0).paramName, "Nothing");
    EXPECT_EQ(mgr.Params().at(1).paramName, "Unknown");
    EXPECT_EQ(mgr.Params().at(2).paramName, "House");
    EXPECT_EQ(mgr.Params().at(2).listType, 1);
}