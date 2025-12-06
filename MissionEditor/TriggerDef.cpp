#include "StdAfx.h"
#include "TriggerDef.h"
#include "inlines.h"
#include "Helpers.h"
#include <format>
#include "TriggerDatabase.h"

const ParamType ParamType::Default;

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
    loadWaypointExceptions(ini);
}

static bool IsWaypointFormat(const CString& s)
{
    if (s.IsEmpty()) {
        return true;
    }
    return s[0] >= 'A' && s[0] <= 'Z';
}

void parseParamList(ParamType& type, const CIniFile& ini, const CString& sec)
{
    auto const& section = ini.GetSection(sec);
    auto const valCount = section.GetInteger("FixedValueCount");
    CString idxStr;
    type.sequencedValues.reserve(valCount);
    for (auto idx = 0; idx < valCount; ++idx) {
        idxStr.Format("%d", idx);
        type.sequencedValues.emplace_back(section.GetString(idxStr));
    }
}

void TriggerDefinitionManager::loadParamTypes(const CIniFile& ini, std::ostream& err)
{
    for (auto const& [id, def] : ini.GetSection("ParamTypes")) {
        auto const paramTypeId = atoi(id);
        auto const params = SplitParams(def);
        int listType = 0;
        bool shouldParseList = true;
        if (IsNumeric(params[1])) {
            listType = atoi(params[1]);
            shouldParseList = false;
        }
        auto const [it, inserted] = m_paramTypes.try_emplace(paramTypeId,
            ParamType{
                .paramName = params[0],
                .listType = listType,
            });
        if (!inserted) {
            err << "Error: duplicated param type definition index " << id << endl;
        }
        // some params will have 3rd control value
        if (params.size() >= 3) {
            it->second.slotCount = atoi(params[2]);
        }
        if (shouldParseList) {
            parseParamList(it->second, ini, params[1]);
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
        if (params.size() < 14) {
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

        item.controlCode = atoi(params[1]);
        for (auto idx = 0; idx < 5; ++idx) {
            if (auto paramType = atoi(params[idx + 2]); paramType != 0) {
                item.paramTypes.emplace(idx, paramType);
            }
        }
        item.useWaypointSlot = atoi(params[7]);
        item.useTag = atoi(params[8]);
        item.obsolete = atoi(params[9]);
        item.tsAllowed = atoi(params[11]);
        item.ra2Allowed = atoi(params[12]);
        // 14th is duplication of action type index again, we verify this value to ensure the definition is not corrupted
        if (atoi(params[13]) != actionTypeId) {
            err << "Error: trigger action definiton corrupted, index: " << id << endl;
            throw std::logic_error("action definition is corrupted");
        }
        // 15th is optional, to check whether allowed in YR
        if (params.size() == 15) {
            item.yrOnly = atoi(params[14]);
        }
    }
}

void TriggerDefinitionManager::loadWaypointExceptions(const CIniFile& ini)
{
    for (auto const [_, actionCode] : ini["DontSaveAsWP"]) {
        m_waypointEncodingExceptions.emplace(atoi(actionCode));
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


TriggerOptions::TriggerOptions(const CString& fullData)
{
    if (fullData.IsEmpty()) {
        return;
    }

    auto const params = SplitParams<7>(fullData);

    house = params[0];
    nextTrigger = params[1];
    name = params[2];
    controls[Disable] = static_cast<bool>(atoi(params[3]));
    controls[Easy] = static_cast<bool>(atoi(params[4]));
    controls[Medium] = static_cast<bool>(atoi(params[5]));
    controls[Hard] = static_cast<bool>(atoi(params[6]));
}

CString TriggerOptions::Serialize() const
{
    CString ret;
    ret.Format("%s,%s,%s,%d,%d,%d,%d,%d",
        house,
        nextTrigger,
        name,
        static_cast<int>(controls[Disable]),
        static_cast<int>(controls[Easy]),
        static_cast<int>(controls[Medium]),
        static_cast<int>(controls[Hard]),
        static_cast<int>(controls[__unused])
    );
    return ret;
}

CString TriggerEvents::Serialize() const
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

TriggerActions::TriggerActions(const CString& fullData)
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
        auto wpStr = params[idx++];
        action.waypoint = IsWaypointFormat(wpStr) ? StringToWaypoint(wpStr) : atoi(wpStr);
        action.actionCode = atoi(actionCodeStr);
        action.lastParamIsWaypoint = TriggerDefinitionManager::Instance().
            IsActionUsingWaypointEncoding(action.actionCode);
    }
}

CString TriggerActions::Serialize() const
{
    constexpr int perEventDataBufferSize = 32;

    CString ret;
    ret.GetBuffer(static_cast<int>(actions.size()) * perEventDataBufferSize);
    ret.Format("%d,", actions.size());
    ret.ReleaseBuffer();

    CString typeStr;
    for (auto const& action : actions) {
        typeStr.Format("%d,%d,", action.ActionType(), action.ActionCode());
        ret += typeStr;
        for (auto const& item : action.Params()) {
            ret += item;
            ret += ',';
        }
        // TODO: adjust according to lastParamIsWaypoint
        ret += action.WaypointString();
        ret += ',';
    }
    ret.TrimRight(',');
    return ret;
}

bool TriggerAction::SetActionType(const int newType)
{
    bool changed = false;
    if (newType != this->actionType) {
        this->actionType = newType;
        changed = true;
    }
    // this type would have new code
    changed |= SetActionCode(abs(Type().controlCode));
    return changed;
}

bool TriggerAction::SetActionCode(const int newCode)
{
    if (this->actionCode == newCode) {
        return false;
    }
    auto const useWpEncoding = TriggerDefinitionManager::Instance().
        IsActionUsingWaypointEncoding(Type());
    this->actionCode = newCode;
    this->lastParamIsWaypoint = useWpEncoding;
    return true;
}

bool TriggerAction::SetWaypoint(const int id)
{
    if (id != waypoint) {
        waypoint = id;
        return true;
    }
    return false;
}

TriggerAction::ParamOperator TriggerAction::ParamNth(int nth)
{
    return ParamOperator(*this, nth);
}

CString TriggerAction::WaypointString() const
{
    auto const& type = Type();
    auto const useWpEncoding = TriggerDefinitionManager::Instance().
        IsActionUsingWaypointEncoding(type);

    if (useWpEncoding) {
        return WaypointToString(waypoint);
    }
    CString ret;
    ret.Format("%d", waypoint);
    return ret;
}

const TriggerActionType& TriggerAction::Type() const
{
    try {
        return TriggerDefinitionManager::Instance().Actions().at(actionType);
    } catch (...) {
        throw std::runtime_error(std::format("action type {} is not registered", actionType));
    }
}

const CString& TriggerAction::ParamOperator::Brief() const
{
    return action.Type().brief;
}

const CString& TriggerAction::ParamOperator::Description() const
{
    return action.Type().description;
}

int TriggerAction::ParamOperator::ListType() const
{
    return type.listType;
}

TriggerAction::ParamOperator::ParamOperator(TriggerAction& action, int nth) :
    action(action),
    type(lookUpParamType(action, nth)),
    nth(nth)
{
}

bool TriggerAction::ParamOperator::Assign(const CString& val)
{
    if (val != action.params[nth]) {
        action.params[nth] = val;
        return true;
    }
    return false;
}

const ParamType& TriggerAction::ParamOperator::lookUpParamType(TriggerAction& action, int nth)
{
    auto const& paramTypes = action.Type().paramTypes;
    if (nth < paramTypes.size()) {
        auto const paramTypeIdx = paramTypes.at(nth);
       return TriggerDefinitionManager::Instance().Params().at(paramTypeIdx);

    }
    return ParamType::Default;
}

TagInstance::TagInstance(const CString& id, const CString& fullData) :
    id(id)
{
    auto const params = SplitParams<3>(fullData);
    persistence = atoi(params[0]);
    name = params[1];
    triggerId= params[2];
}

CString TagInstance::Serialize() const
{
    CString ret;
    ret.Format("%d,%s,%s", persistence, name, triggerId);
    return ret;
}
