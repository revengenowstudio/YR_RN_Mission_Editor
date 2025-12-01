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
60=Techno Exists,48,46,0,0,So many techno type exists on map,0,1,60,1

[ActionsRA2]
0=-Nothing-,0,0,0,0,0,0,0,0,0,This is an empty action. It means doing nothing.,0,1,0
1=Claim Winner...,0,2,0,0,0,0,0,0,0,The winner must be a specific side%1 and the game will end immediately. For example%1 the players on a specific side are defined. In a multiplayer task%1 this action will lead to the failure of all players.,0,1,1
2=Claim Loser...,0,2,0,0,0,0,0,0,0,The loser must be a specific side%1 and the game will end immediately. When a specific side is designated as the loser%1 the game ends right away. For example%1 the players on a specific side are defined. If a non-player country is set as the loser%1 it can result in a mission victory. a typical example is a modified mod that changes the order of countries. In a multiplayer campaign%1 this action will lead to the failure of all players.,0,1,2
4=Create Team...,-1,7,6,0,0,0,0,0,0,Create a New TeamType instance,0,1,4
129=Set SW Charge Percentage,-11,20,0,0,0,0,1,0,0,This will set owner's Superweapon percentage,0,1,129
130=Restore Initial Buildings...,0,2,0,0,0,0,0,0,0,All buildings of selected house's will be rebuilt,0,1,130,1
)";

    CIniFile ini;
    EXPECT_EQ(ini.InsertStream(testIniContent), 0);

    auto& mgr = TriggerDefinitionManager::Instance();
    mgr.LoadFrom(ini, std::cerr);

    EXPECT_EQ(mgr.Actions().size(), 6);
    EXPECT_EQ(mgr.Actions().at(0).description, "This is an empty action. It means doing nothing.");
    EXPECT_EQ(mgr.Actions().at(1).controlCode, 0);
    EXPECT_EQ(mgr.Actions().at(1).paramTypes.at(0), 2);
    EXPECT_EQ(mgr.Actions().at(1).paramTypes.size(), 1);
    EXPECT_EQ(mgr.Actions().at(1).actionType, 1);
    EXPECT_EQ(mgr.Actions().at(1).ra2Allowed, true);
    EXPECT_EQ(mgr.Actions().at(1).yrOnly, false);
    EXPECT_EQ(mgr.Actions().at(4).controlCode, -1);
    EXPECT_EQ(mgr.Actions().at(4).paramTypes.at(0), 7);
    EXPECT_EQ(mgr.Actions().at(129).ra2Allowed, true);
    EXPECT_EQ(mgr.Actions().at(129).useWaypointSlot, true);
    EXPECT_EQ(mgr.Actions().at(129).yrOnly, false);
    EXPECT_EQ(mgr.Actions().at(130).ra2Allowed, true);
    EXPECT_EQ(mgr.Actions().at(130).yrOnly, true);

    EXPECT_EQ(mgr.Events().size(), 7);
    EXPECT_EQ(mgr.Events().at(0).description, "This is an empty event.");
    EXPECT_EQ(mgr.Events().at(2).obsolete, true);
    EXPECT_EQ(mgr.Events().at(8).description, "When use it alone, take effect immediately");
    EXPECT_EQ(mgr.Events().at(60).paramTypes[0], 46); // attention, ohhhhhh
    EXPECT_EQ(mgr.Events().at(60).paramTypes[1], 48); // attention, ohhhhhh

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

TEST(TriggerActionTest, ActionSerde)
{

    CIniFile wpFilterIni;
    auto& filterSec = wpFilterIni.AddSection("DontSaveAsWP");
    filterSec.SetString("0", "5");
    filterSec.SetString("1", "9");
    filterSec.SetString("3", "11");

    auto filterFunc = [&wpFilterIni](const CString& id) {
        return !wpFilterIni["DontSaveAsWP"].HasValue(id);
    };
    // 2 events
    {
        const CString data = "2,11,4,mission:usa01_07,0,0,0,0,A,53,2,01000020,0,0,0,0,A";
        TriggerActions actions(data, filterFunc);


        EXPECT_EQ(actions.Size(), 2);
        EXPECT_EQ(actions.Nth(0).actionType, 11);
        EXPECT_EQ(actions.Nth(0).actionCode, 4);
        EXPECT_EQ(actions.Nth(0).params[0], "mission:usa01_07");
        EXPECT_EQ(actions.Nth(1).actionType, 53);
        EXPECT_EQ(actions.Nth(1).actionCode, 2);
        EXPECT_EQ(actions.Nth(1).params[0], "01000020");

        EXPECT_EQ(actions.Serialize(), data);
    }
    // 7 events
    {
        const CString data = "7,3,0,9,0,0,0,0,A,13,0,9,0,0,0,0,A,74,0,9,0,0,0,0,A,53,2,01000413,0,0,0,0,A,53,2,01000424,0,0,0,0,A,76,0,50,0,0,0,0,A,53,2,01000553,0,0,0,0,A";
        TriggerActions actions(data, filterFunc);

        EXPECT_EQ(actions.Size(), 7);
        EXPECT_EQ(actions.Nth(0).actionType, 3);
        EXPECT_EQ(actions.Nth(1).actionType, 13);
        EXPECT_EQ(actions.Nth(2).actionType, 74);
        EXPECT_EQ(actions.Nth(3).actionType, 53);
        EXPECT_EQ(actions.Nth(4).actionType, 53);
        EXPECT_EQ(actions.Nth(5).actionType, 76);
        EXPECT_EQ(actions.Nth(6).params[0], "01000553");
        EXPECT_EQ(actions.Nth(6).actionType, 53);

        EXPECT_EQ(actions.Serialize(), data);
    }

    // action 129
    {
        const CString data = "4,129,11,31,0,0,0,0,91,11,4,mission:usa01_21,0,0,0,0,A,19,7,WarningAlarm,0,0,0,0,A,53,2,01000042,0,0,0,0,A";
        TriggerActions actions(data, filterFunc);

        EXPECT_EQ(actions.Size(), 4);
        EXPECT_EQ(actions.Nth(0).lastParamIsWaypoint, false);
        EXPECT_EQ(actions.Nth(0).waypoint, "91");
    }

}