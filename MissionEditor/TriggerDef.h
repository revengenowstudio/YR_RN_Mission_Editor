#pragma once

#include <map>
#include <array>
#include <ostream>
#include <optional>
#include <functional>
#include <unordered_set>

class CIniFile;
class TriggerEvents;
class TriggerActions;

struct ParamType 
{
    static const ParamType Default;

    CString paramName;
    int listType{ 0 }; // PARAMTYPE_NOTHING
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
    static constexpr int WaypointSlot = 5;

    int actionType;                 // same as enumeration defined in game, maybe unnecessary, can be optimized out when feature done
    CString brief;                  // brief name shows what it is         
    CString description;            // desc about usage
    int controlCode;                // negative value, should turn back
    std::map<int, int> paramTypes; // position : paramType
    bool useWaypointSlot{ false };     // 6th value not waypoint
    bool useTag{ false };
    bool obsolete{ false };
    bool tsAllowed{ true };
    bool ra2Allowed{ true };
    bool yrOnly{ false };
};

struct TriggerOptions
{
    enum Controls {
        Disable,
        Easy,
        Medium,
        Hard,
        __unused,
        __ControlCount,
    };

    TriggerOptions(const CString& fullData);

    CString Serialize() const;

    CString house;
    CString nextTrigger{ "<none>" };
    CString name;
    bool controls[__ControlCount] {
        false,
        true,
        true,
        true,
        false,
    };
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

    CString Serialize() const;

private:
    std::vector<TriggerEvent> events;
};

// TODO: change to class, use methods to access member and trigger def info
class TriggerAction
{
public:
    class ParamOperator
    {
    public:
        const CString& Brief() const;
        const CString& Description() const;
        int ListType() const;

        ParamOperator(TriggerAction& action, int nth);

        bool Assign(const CString& val);

    private:
        static const ParamType& lookUpParamType(TriggerAction& action, int nth);

        TriggerAction& action;
        const ParamType& type;
        const int nth;
    };

    TriggerAction() :
        actionType(0),
        actionCode(0),
        params {
            '0', '0', '0', '0', '0',
        }
    {}

    bool SetActionType(const int newType);
    bool SetActionCode(const int newCode);
    bool SetWaypoint(const int id);
    ParamOperator ParamNth(int nth);

    auto const ActionType() const { return actionType; } // TODO: rename to TypeIndex
    auto const ActionCode() const { return actionCode; }
    const int Waypoint() const { return waypoint; }
    CString WaypointString() const;
    auto const& Params() const { return params; }
    auto const IsUsingWaypointEncoding() const { return lastParamIsWaypoint; }

    const TriggerActionType& Type() const;

private:
    friend class TriggerActions;

    int actionType{ 0 };
    int actionCode{ 0 }; // no idea yet, implies how param slots are used
    std::array<CString, 5> params;
    int waypoint{ 0 }; // use WaypointString to get correctly encoded data
    bool lastParamIsWaypoint{ true };
};

// TODO: refine TriggerEvents and TriggerActions from same template
class TriggerActions
{
public:
    TriggerActions(const CString& fullData);

    auto Size() const { return actions.size(); }
    TriggerAction& Nth(size_t idx) { return actions.at(idx); }
    void DeleteAt(size_t idx) { actions.erase(actions.begin() + idx); }

    TriggerAction& Insert(size_t slot, TriggerAction&& event = {}) {
        return *actions.emplace(actions.begin() + slot, std::move(event));
    }

    CString Serialize() const;

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

    bool IsActionUsingWaypointEncoding(const int controlCode) const
    {
        return !m_waypointEncodingExceptions.contains(controlCode);
    }
    bool IsActionUsingWaypointEncoding(const TriggerActionType& actionType) const
    {
        return IsActionUsingWaypointEncoding(abs(actionType.controlCode));
    }


private:
    void loadParamTypes(const CIniFile& ini, std::ostream& err);
    void loadEventTypes(const CIniFile& ini, std::ostream& err);
    void loadActionTypes(const CIniFile& ini, std::ostream& err);
    void loadWaypointExceptions(const CIniFile& ini);

    // we need asc order of ids
    std::map<int, ParamType> m_paramTypes;
    std::map<int, TriggerEventType> m_eventTypes;
    std::map<int, TriggerActionType> m_actionTypes;
    std::unordered_set<int> m_waypointEncodingExceptions;
};
