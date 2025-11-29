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
        CString desc = params[10];
        desc.Replace("%1", ",");

        item.actionType = actionTypeId;
        item.brief = params[0];
        item.description = desc;

        for (auto idx = 0; idx < 6; ++idx) {
            item.paramTypes[idx] = atoi(params[idx + 1]);
        }
        // 13th is optional, to check whether allowed in YR

    }
}
