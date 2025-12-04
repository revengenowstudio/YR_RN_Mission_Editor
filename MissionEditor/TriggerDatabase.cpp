#include "StdAfx.h"
#include "TriggerDatabase.h"

static auto constexpr SEC_TRIGGERS = "Triggers";
static auto constexpr SEC_EVENTS = "Events";
static auto constexpr SEC_ACTIONS = "Actions";

const TriggerInstance TriggerInstance::Default {
    "0"
};

TriggerDatabase& TriggerDatabase::Instance()
{
    static TriggerDatabase inst;
    return inst;
}

TriggerInstance& TriggerDatabase::Lookup(const CString& id)
{
    auto const it = lookupTable.find(id);
    if (it != lookupTable.end()) {
        return items.at(it->second);
    }
    throw std::runtime_error("no such trigger");
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
    return items.at(idx);
}

void TriggerDatabase::Append(TriggerInstance&& inst)
{
    lookupTable.insert_or_assign(inst.ID(), items.size());
    items.emplace_back(std::move(inst));
}

TriggerInstance& TriggerDatabase::Append(const CString& id, CString&& name)
{
    lookupTable.insert_or_assign(id, items.size());
    auto& ret = items.emplace_back(id);
    ret.Options().name = std::move(name);
    return ret;
}

void TriggerDatabase::DeleteAt(size_t idx)
{
    auto const it = items.erase(items.begin() + idx);
    ASSERT(idx < items.size());
    // delete from record first;
    auto const& trigger = items.at(idx);
    auto const eraseCount = lookupTable.erase(trigger.ID());
    ASSERT(eraseCount == 1);
    items.erase(items.begin() + idx);
    // now update all key-pos indexing, dec 1
    for (auto affectedIdx = idx; affectedIdx < items.size(); ++affectedIdx) {
        auto const& triggerN = items[affectedIdx];
        auto const it = lookupTable.find(triggerN.ID());
        ASSERT(it != lookupTable.end());
        it->second--;
    }
}

void TriggerDatabase::LoadFrom(const CIniFile& ini, std::ostream& err)
{
    auto const& triggerSec = ini[SEC_TRIGGERS];
    items.reserve(triggerSec.Size());

    for (auto const& [id, opts] : triggerSec) {
        items.emplace_back(id, ini);
        lookupTable.try_emplace(id, items.size() - 1);
    }
}

void TriggerDatabase::SaveInto(CIniFile& ini, std::ostream& err)
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
