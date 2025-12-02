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

    std::stringstream testIniContent;
    testIniContent <<
        R"(
[DontSaveAsWP]
0=5
1=9
;2=10
3=11

[ParamTypes]
-1=Unused,0,1;not listed in FA2
0=Nothing,0
1=Unknown,0
2=Affiliated Side,1
3=Local Variable,20
4=Time,0
5=Score,0
6=Value,0
7=Squad Type,2
8=Building,6
9=Aircraft,5
10=Infantry,4
11=Unit,3
12=Movie,7
13=Text,8
14=Trigger,9
15=Allowed,10
16=Sound,11
17=Music,12
18=Voice,13
19=Step,0
20=Super Weapon,14
21=Left,0
22=Top,0
23=Width,0
24=Height,0
25=Animation,15
26=Particle,16
27=Duration,0
28=Speed,0
29=Voxel Fragment ID,29
30=Way Point,17
31=Wooden Crate Type,18
32=Voice Prompt Box,19
33=Character,21
34=Action,9
35=Global Variable,27
36=Specific Weapon,14
37=Activated,10
38=Trigger Tag,22
39=Technology Type,0
40=Source,0
41=Weapon,24
42=Glow Behavior,25
43=Event,9
44=Rain,26
45=Float Value,0
46=Technology Type,29
47=Building,28
48=Value,0,2
49=Pixel Animation,23
50=Film,7

[ActionsRA2]
3=Begin production...,0,2,0,0,0,0,0,0,0,AI begin production,0,1,3
11=CSF Text...,-4,13,2,6,0,0,0,0,0,You know the usage,0,1,11
13=(Unused)Auto create starts...,0,2,0,0,0,0,0,0,0,AI starts auto create,0,1,13
53=Enable trigger,-2,14,0,0,0,0,0,0,0,Enable a trigger,0,1,53
55=Create radar event,0,43,2,0,0,0,1,0,0,Create radar event at waypoint,0,1,55
74=AI Trigger begins...,0,2,0,0,0,0,0,0,0,Enable house AI,0,1,74
76=AI team rating...,0,6,0,0,0,0,0,0,0,AI global trigger preference rate,0,1,76
129=Set SW Charge Percentage,-11,20,0,0,0,0,1,0,0,This will set owner's Superweapon percentage,0,1,129
)";

    CIniFile ini;
    EXPECT_EQ(ini.InsertStream(testIniContent), 0);

    auto& mgr = TriggerDefinitionManager::Instance();
    mgr.LoadFrom(ini, std::cerr);

    auto filterFunc = [&ini](const CString& id) {
        return !ini["DontSaveAsWP"].HasValue(id);
    };
    // 2 events
    {
        const CString data = "2,11,4,mission:all01_07,0,0,0,0,A,53,2,01000020,0,0,0,0,A";
        TriggerActions actions(data);


        EXPECT_EQ(actions.Size(), 2);
        EXPECT_EQ(actions.Nth(0).ActionType(), 11);
        EXPECT_EQ(actions.Nth(0).ActionCode(), 4);
        EXPECT_EQ(actions.Nth(0).Params()[0], "mission:all01_07");
        EXPECT_EQ(actions.Nth(1).ActionType(), 53);
        EXPECT_EQ(actions.Nth(1).ActionCode(), 2);
        EXPECT_EQ(actions.Nth(1).Params()[0], "01000020");

        EXPECT_EQ(actions.Serialize(), data);
    }
    // 7 events
    {
        const CString data = "7,3,0,9,0,0,0,0,A,13,0,9,0,0,0,0,A,74,0,9,0,0,0,0,A,53,2,01000413,0,0,0,0,A,53,2,01000424,0,0,0,0,A,76,0,50,0,0,0,0,A,53,2,01000553,0,0,0,0,A";
        TriggerActions actions(data);

        EXPECT_EQ(actions.Size(), 7);
        EXPECT_EQ(actions.Nth(0).ActionType(), 3);
        EXPECT_EQ(actions.Nth(1).ActionType(), 13);
        EXPECT_EQ(actions.Nth(2).ActionType(), 74);
        EXPECT_EQ(actions.Nth(3).ActionType(), 53);
        EXPECT_EQ(actions.Nth(4).ActionType(), 53);
        EXPECT_EQ(actions.Nth(5).ActionType(), 76);
        EXPECT_EQ(actions.Nth(6).Params()[0], "01000553");
        EXPECT_EQ(actions.Nth(6).ActionType(), 53);

        EXPECT_EQ(actions.Serialize(), data);
    }

    // action 129
    {
        const CString data = "4,129,11,31,0,0,0,0,91,11,4,mission:all01_21,0,0,0,0,A,19,7,WarningAlarm,0,0,0,0,A,53,2,01000042,0,0,0,0,A";
        TriggerActions actions(data);

        EXPECT_EQ(actions.Size(), 4);
        EXPECT_EQ(actions.Nth(0).IsUsingWaypointEncoding(), false);
        EXPECT_EQ(actions.Nth(0).WaypointString(), "91");
    }

    // action 55
    {
        const CString data = "3,19,7,WarningAlarm,0,0,0,0,A,11,4,mission:all01_13,0,0,0,0,A,55,0,0,0,0,0,0,J";
        TriggerActions actions(data);

        EXPECT_EQ(actions.Size(), 3);
        EXPECT_EQ(actions.Nth(2).IsUsingWaypointEncoding(), true);
        EXPECT_EQ(actions.Nth(2).Waypoint(), 9);
        EXPECT_EQ(actions.Nth(2).WaypointString(), "J");
    }
}