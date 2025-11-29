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
48=Value,0,2

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

    EXPECT_EQ(mgr.Params().size(), 5);
    EXPECT_EQ(mgr.Params().at(-1).paramName, "Unused");
    EXPECT_EQ(mgr.Params().at(0).paramName, "Nothing");
    EXPECT_EQ(mgr.Params().at(1).paramName, "Unknown");
    EXPECT_EQ(mgr.Params().at(2).paramName, "House");
    EXPECT_EQ(mgr.Params().at(2).listType, 1);
    EXPECT_EQ(mgr.Params().at(48).listType, 0);
    EXPECT_EQ(mgr.Params().at(48).slotCount, 2);
}

TEST(TriggerEventTest, EventsSerde)
{
    // single event
    {
        const CString data = "1,13,0,10";
        TriggerEvents events(data);

        EXPECT_EQ(events.Size(), 1);
        EXPECT_EQ(events.Nth(0).eventType, 13);
        EXPECT_EQ(events.Nth(0).param1, "10");
        EXPECT_EQ(events.Nth(0).param2, std::nullopt);

        EXPECT_EQ(events.Serialize(), data);
    }
    // 2 simple events
    {
        const CString data = "2,36,0,4,13,0,5";
        TriggerEvents events(data);

        EXPECT_EQ(events.Size(), 2);
        EXPECT_EQ(events.Nth(0).eventType, 36);
        EXPECT_EQ(events.Nth(0).param1, "4");
        EXPECT_EQ(events.Nth(0).param2, std::nullopt);
        EXPECT_EQ(events.Nth(1).eventType, 13);
        EXPECT_EQ(events.Nth(1).param1, "5");
        EXPECT_EQ(events.Nth(1).param2, std::nullopt);

        EXPECT_EQ(events.Serialize(), data);
    }
    // 1 event with double params
    {
        const CString data = "1,61,2,1,GAPOWR";
        TriggerEvents events(data);

        EXPECT_EQ(events.Size(), 1);
        EXPECT_EQ(events.Nth(0).eventType, 61);
        EXPECT_EQ(events.Nth(0).param1, "1");
        EXPECT_EQ(events.Nth(0).param2, "GAPOWR");

        EXPECT_EQ(events.Serialize(), data);
    }
    // 3 events with double params in one event
    {
        const CString data = "3,61,2,1,GAPOWR,37,0,8,37,0,9";
        TriggerEvents events(data);

        EXPECT_EQ(events.Size(), 3);
        EXPECT_EQ(events.Nth(0).eventType, 61);
        EXPECT_EQ(events.Nth(0).param1, "1");
        EXPECT_EQ(events.Nth(0).param2, "GAPOWR");
        EXPECT_EQ(events.Nth(1).eventType, 37);
        EXPECT_EQ(events.Nth(1).param1, "8");
        EXPECT_EQ(events.Nth(1).param2, std::nullopt);
        EXPECT_EQ(events.Nth(2).eventType, 37);
        EXPECT_EQ(events.Nth(2).param1, "9");
        EXPECT_EQ(events.Nth(2).param2, std::nullopt);

        EXPECT_EQ(events.Serialize(), data);
    }
}