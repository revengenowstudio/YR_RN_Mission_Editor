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

struct TriggerActionType {
    int actionType;                 // same as enumeration defined in game, maybe unnecessary, can be optimized out when feature done
    CString brief;                  // brief name shows what it is         
    CString description;            // desc about usage
    std::array<int, 6> paramTypes;
    bool isWaypoint;
    bool saveAsWaypoint;                  
};

class TriggerDefinitionManager
{
public:
    static TriggerDefinitionManager& Instance();

    void LoadFrom(const CIniFile& ini, std::ostream& err);

    auto const& Actions() const { return m_actionTypes; }
    auto const& Params() const { return m_paramTypes; }

private:
    void loadParamTypes(const CIniFile& ini, std::ostream& err);
    void loadActionTypes(const CIniFile& ini, std::ostream& err);

    std::unordered_map<int, ParamType> m_paramTypes;
    std::unordered_map<int, TriggerActionType> m_actionTypes;
};
