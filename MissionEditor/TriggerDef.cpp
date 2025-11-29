#include "StdAfx.h"
#include "TriggerDef.h"
#include "inlines.h"

TriggerDefinitionManager& TriggerDefinitionManager::Instance()
{
    static TriggerDefinitionManager inst;
    return inst;
}

void TriggerDefinitionManager::LoadFrom(const CIniFile& ini, std::ostream& err)
{
    loadParamTypes(ini, err);
    loadEventTypes(ini, err);
    loadActionTypes(ini, err);
}

void TriggerDefinitionManager::loadParamTypes(const CIniFile& ini, std::ostream& err)
{
    for (auto const& [id, def] : ini.GetSection("ParamTypes")) {
        auto const paramTypeId = atoi(id);
        auto const params = SplitParams(def);
        auto const [it, inserted] = m_paramTypes.try_emplace(paramTypeId,
            ParamType{
                .paramName = params[0],
                .listType = atoi(params[1]),
            });
        if (!inserted) {
            err << "Error: duplicated param type definition index " << id << endl;
        }
    }
}

void TriggerDefinitionManager::loadEventTypes(const CIniFile& ini, std::ostream& err)
{
#ifdef RA2_MODE
#define EVENT_DEF_SEC "EventsRA2"
#else
#define EVENT_DEF_SEC "Events"
#endif

    for (auto const& [id, def] : ini.GetSection(EVENT_DEF_SEC)) {
        auto const params = SplitParams(def);

        if (params.size() < 9) {
            err << "Error: trigger action segment error, should be 6 segments, index: " << id << endl;
            continue;
        }

        auto const eventTypeId = atoi(id);
        auto const [it, inserted] = m_eventTypes.try_emplace(eventTypeId, TriggerEventType{});
        if (!inserted) {
            err << "Error: duplicated trigger event definition index " << id << endl;
            continue;
        }

        auto& item = it->second;
        item.brief.Format("%s %s", id, params[0]);
        item.brief.Replace("%1", ",");

        item.paramTypes[0] = atoi(params[1]);
        item.paramTypes[1] = atoi(params[2]);
        item.tagNeeded = atoi(params[3]);
        item.obsolete = atoi(params[4]);

        item.description = params[5];
        item.description.Replace("%1", ",");

        item.tsAllowed = atoi(params[6]);
        item.ra2Allowed = atoi(params[7]);
        // 9th is duplication of event type index again, we skip it
        // 10th is optional, to check whether allowed in YR
        if (params.size() == 10) {
            item.yrOnly = atoi(params[9]);
        }
    }
}

void TriggerDefinitionManager::loadActionTypes(const CIniFile& ini, std::ostream& err)
{
#ifdef RA2_MODE
#define ACTION_DEF_SEC "ActionsRA2"
#else
#define ACTION_DEF_SEC "Actions"
#endif

    for (auto const& [id, def] : ini.GetSection(ACTION_DEF_SEC)) {
        auto const params = SplitParams(def);
        if (params.size() < 12) {
            err << "Error: trigger action segment error, should be 6 segments, index: " << id << endl;
            continue;
        }

        auto const actionTypeId = atoi(id);
        auto const [it, inserted] = m_actionTypes.try_emplace(actionTypeId, TriggerActionType{});
        if (!inserted) {
            err << "Error: duplicated trigger action definition index " << id << endl;
            continue;
        }

        auto& item = it->second;
        item.actionType = actionTypeId;
        item.brief.Format("%s %s", id, params[0]);
        item.description = params[10];
        item.description.Replace("%1", ",");

        for (auto idx = 0; idx < 6; ++idx) {
            item.paramTypes[idx] = atoi(params[idx + 1]);
        }
        item.useWaypoint = atoi(params[6]);
        item.useTag = atoi(params[7]);
        item.obsolete = atoi(params[8]);
        item.tsAllowed = atoi(params[9]);
        item.ra2Allowed = atoi(params[10]);
        // 11th is duplication of action type index again, we skip it
        // 13th is optional, to check whether allowed in YR
        if (params.size() == 13) {
            item.yrOnly = atoi(params[12]);
        }
    }
}
