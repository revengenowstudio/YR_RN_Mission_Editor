#pragma once

#include <map>
#include <array>
#include <ostream>
#include <optional>
#include <functional>

class CIniFile;
class TriggerEvents;

struct ParamType 
{
    CString paramName;
    int listType{ 0 };
    int slotCount{ 0 }; // usually 0. Seems 2 is specially meaningful
};

struct TriggerEventType
{
    CString brief;                  // brief name shows what it is         
    CString description;            // desc about usage
    std::array<int, 2> paramTypes; // 0 means this slot not used
    bool tagNeeded{ false };
    bool obsolete{ false };
    bool tsAllowed{ true };
    bool ra2Allowed{ true };
    bool yrOnly{ false };
};

struct TriggerActionType 
{
    int actionType;                 // same as enumeration defined in game, maybe unnecessary, can be optimized out when feature done
    CString brief;                  // brief name shows what it is         
    CString description;            // desc about usage
    std::array<int, 6> paramTypes;
    bool useWaypoint{ false };
    bool useTag{ false };
    bool obsolete{ false };
    bool tsAllowed{ true };
    bool ra2Allowed{ true };
    bool yrOnly{ false };
};

struct TriggerEvent
{
    int eventType{ 0 };
    CString param1;
    std::optional<CString> param2;
};

class TriggerEvents
{
public:
    TriggerEvents(const CString& fullData);

    auto Size() const { return events.size(); }

    TriggerEvent& Nth(size_t idx) { return events.at(idx); }
    void DeleteAt(size_t idx) { events.erase(events.begin() + idx); }

    TriggerEvent& Insert(size_t slot, TriggerEvent&& event = {}) {
        return *events.emplace(events.begin() + slot, std::move(event));
    }

    CString Serialize();

private:
    std::vector<TriggerEvent> events;
};

struct TriggerAction
{
    int actionType{ 0 };
    int actionCode{ 0 }; // no idea yet, implies how param slots are used
    std::array<CString, 5> params;
    CString waypoint{ 'A' }; // here 'A' means 0
    bool lastParamIsWaypoint{ true };
};

// TODO: refine TriggerEvents and TriggerActions from same template
class TriggerActions
{
public:
    // return true means is waypoint
    using WpFilterFunc = std::function<bool(const CString& actionType)>;

    TriggerActions(const CString& fullData, const WpFilterFunc& filter);

    auto Size() const { return actions.size(); }
    TriggerAction& Nth(size_t idx) { return actions.at(idx); }
    void DeleteAt(size_t idx) { actions.erase(actions.begin() + idx); }

    TriggerAction& Insert(size_t slot, TriggerAction&& event = {}) {
        return *actions.emplace(actions.begin() + slot, std::move(event));
    }

    CString Serialize();

private:
    std::vector<TriggerAction> actions;
};

class TriggerDefinitionManager
{
public:
    static TriggerDefinitionManager& Instance();

    void LoadFrom(const CIniFile& ini, std::ostream& err);

    auto const& Actions() const { return m_actionTypes; }
    auto const& Events() const { return m_eventTypes; }
    auto const& Params() const { return m_paramTypes; }

private:
    void loadParamTypes(const CIniFile& ini, std::ostream& err);
    void loadEventTypes(const CIniFile& ini, std::ostream& err);
    void loadActionTypes(const CIniFile& ini, std::ostream& err);

    // we need asc order of ids
    std::map<int, ParamType> m_paramTypes;
    std::map<int, TriggerEventType> m_eventTypes;
    std::map<int, TriggerActionType> m_actionTypes;
};
