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
        // some params will have 3rd control value
        if (params.size() >= 3) {
            it->second.slotCount = atoi(params[2]);
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
        item.brief.Format("%s -%s", id, params[0]);
        item.brief.Replace("%1", ",");
        // attention: it really sucks here, P1 P2 actually reversed
        item.paramTypes[0] = atoi(params[2]);
        item.paramTypes[1] = atoi(params[1]);
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
        item.brief.Format("%s -%s", id, params[0]);
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

TriggerEvents::TriggerEvents(const CString& fullData)
{
    if (fullData.IsEmpty()) {
        return;
    }
    // deserialize events
    size_t idx = 0;
    auto const params = SplitParams(fullData);
    size_t eventCount = atoi(params[idx++]); // first param means event count
    events.reserve(eventCount);

    for (;;) {
        if (idx >= params.size()) {
            break;
        }
        auto eventType = atoi(params[idx++]);
        auto const slots = atoi(params[idx++]);
        auto p1 = params[idx++];
        p1.Trim();
        auto&& p2 = slots == 2 ? params[idx++] : decltype(TriggerEvent::param2)(std::nullopt);
        if (p2.has_value()) {
            p2->Trim();
        }
        events.emplace_back(
            eventType,
            p1,
            p2
           );
    }
}

CString TriggerEvents::Serialize()
{
    constexpr int perEventDataBufferSize = 32;

    CString ret;
    ret.GetBuffer(static_cast<int>(events.size()) * perEventDataBufferSize);
    ret.Format("%d,", events.size());
    ret.ReleaseBuffer();

    CString typeStr;
    for (auto const& event : events) {
        typeStr.Format("%d,", event.eventType);
        ret += typeStr;
        ret += event.param2.has_value() ? '2' : '0';
        ret += ',';
        ret += event.param1;
        ret += ',';
        if (event.param2.has_value()) {
            ret += event.param2.value();
            ret += ',';
        }
    }
    ret.TrimRight(',');
    return ret;
}

TriggerActions::TriggerActions(const CString& fullData, const WpFilterFunc& filter)
{
    if (fullData.IsEmpty()) {
        return;
    }

    size_t idx = 0;
    auto const params = SplitParams(fullData);
    size_t actionCount = atoi(params[idx++]); // first param means event count
    actions.reserve(actionCount);

    // each action contains 8 segments
    if (params.size() != actionCount * 8 + 1) {
        throw std::invalid_argument("action data corrupted");
    }

    for (;;) {
        if (idx >= params.size()) {
            break;
        }

        auto& action = actions.emplace_back();

        action.actionType = atoi(params[idx++]);
        auto actionCodeStr = params[idx++];
        for (auto& param : action.params) {
            param = params[idx++];
        }
        action.waypoint = params[idx++];
        action.lastParamIsWaypoint = filter(actionCodeStr);
        action.actionCode = atoi(actionCodeStr);
    } 
}

CString TriggerActions::Serialize()
{
    constexpr int perEventDataBufferSize = 32;

    CString ret;
    ret.GetBuffer(static_cast<int>(actions.size()) * perEventDataBufferSize);
    ret.Format("%d,", actions.size());
    ret.ReleaseBuffer();

    CString typeStr;
    for (auto const& action : actions) {
        typeStr.Format("%d,%d,", action.actionType, action.actionCode);
        ret += typeStr;
        for (auto const& item : action.params) {
            ret += item;
            ret += ',';
        }
        // TODO: adjust according to lastParamIsWaypoint
        ret += action.waypoint;
        ret += ',';
    }
    ret.TrimRight(',');
    return ret;
}
