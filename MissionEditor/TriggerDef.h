#pragma once

#include <unordered_map>
#include <array>
#include <ostream>

class CIniFile;

struct ParamType 
{
    CString paramName;
    int listType{ 0 };
};

struct TriggerEventType
{
    CString brief;                  // brief name shows what it is         
    CString description;            // desc about usage
    std::array<int, 2> paramTypes;
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

    std::unordered_map<int, ParamType> m_paramTypes;
    std::unordered_map<int, TriggerEventType> m_eventTypes;
    std::unordered_map<int, TriggerActionType> m_actionTypes;
};
