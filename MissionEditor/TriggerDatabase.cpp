#include "StdAfx.h"
#include "TriggerDatabase.h"

static auto constexpr SEC_EVENTS = "Events";
static auto constexpr SEC_ACTIONS = "Actions";

TriggerDatabase& TriggerDatabase::Instance()
{
    static TriggerDatabase inst;
    return inst;
}

std::optional<TriggerInstance&> TriggerDatabase::Lookup(const CString& id)
{
    auto const it = lookupTable.find(id);
    if (it != lookupTable.end()) {
        return items.at(it->second);
    }
    return std::nullopt;
}

TriggerInstance& TriggerDatabase::InsertAt(size_t idx, CString&& key)
{
    if (idx > items.size()) {
        idx = items.size() - 1;
    }
    items.insert(items.begin() + idx, { key });
    lookupTable.insert_or_assign(key, idx);
    // fix all indexes
    for (auto it = lookupTable.upper_bound(key); it != lookupTable.end(); ++it) {
        it->second++;
    }
}

void TriggerDatabase::LoadFrom(const CIniFile& ini, std::ostream& err)
{
    auto const& triggerSec = ini["Triggers"];
    items.reserve(triggerSec.Size());

    for (auto const& [id, opts] : triggerSec) {
        items.emplace_back(id, ini);
        lookupTable.try_emplace(id, items.size() - 1);
    }
}

void TriggerDatabase::SaveInto(CIniFile& ini, std::ostream& err)
{
    auto& triggerSec = ini.AddSection("Triggers");
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
    options(),
    events({}),
    actions({})
{ }

TriggerInstance::TriggerInstance(const CString& id, const CIniFile& ini) :
    options(),
    events(ini.GetString(SEC_EVENTS, id)),
    actions(ini.GetString(SEC_ACTIONS, id))
{
}
