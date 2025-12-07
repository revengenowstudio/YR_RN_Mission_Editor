#include "StdAfx.h"
#include "TriggerDatabase.h"

static auto constexpr SEC_TRIGGERS = "Triggers";
static auto constexpr SEC_EVENTS = "Events";
static auto constexpr SEC_ACTIONS = "Actions";

const TriggerInstance TriggerInstance::Default {
    "0"
};

template<>
ObjectDatabase<TriggerInstance>& ObjectDatabase<TriggerInstance>::Instance()
{
    static ObjectDatabase inst;
    return inst;
}

template<>
void ObjectDatabase<TriggerInstance>::LoadFrom(const CIniFile& ini, std::ostream& err)
{
    Clear();
    auto const& triggerSec = ini[SEC_TRIGGERS];
    items.reserve(triggerSec.Size());

    for (auto const& [id, opts] : triggerSec) {
        items.emplace_back(id, ini);
        lookupTable.try_emplace(id, items.size() - 1);
    }
}

template<>
void ObjectDatabase<TriggerInstance>::SaveInto(CIniFile& ini, std::ostream& err)
{
    auto& triggerSec = ini.AddSection(SEC_TRIGGERS);
    auto& eventsSec = ini.AddSection(SEC_EVENTS);
    auto& actionsSec = ini.AddSection(SEC_ACTIONS);
    triggerSec.Clear();

    for (auto const& trigger : items) {
        triggerSec.SetString(trigger.ID(), trigger.Options().Serialize());
        eventsSec.SetString(trigger.ID(), trigger.Events().Serialize());
        actionsSec.SetString(trigger.ID(), trigger.Actions().Serialize());
    }
}

TriggerInstance::TriggerInstance(const CString& id) :
    id(id),
    options({}),
    events({}),
    actions({})
{
}

TriggerInstance::TriggerInstance(const CString& id, const CIniFile& ini) :
    id(id),
    options(ini.GetString(SEC_TRIGGERS, id)),
    events(ini.GetString(SEC_EVENTS, id)),
    actions(ini.GetString(SEC_ACTIONS, id))
{
}

TriggerInstance::TriggerInstance(CString&& id, CString&& name, CString&& house) :
    TriggerInstance(id)
{
    this->Options().name = std::move(name);
    this->Options().house = std::move(house);
}
