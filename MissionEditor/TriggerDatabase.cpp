#include "StdAfx.h"
#include "TriggerDatabase.h"

static auto constexpr SEC_TRIGGERS = "Triggers";
static auto constexpr SEC_EVENTS = "Events";
static auto constexpr SEC_ACTIONS = "Actions";
static auto constexpr SEC_TAGS = "Tags";

const TriggerInstance TriggerInstance::Default{ "0" };

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

    if (indexUpdateHandler) {
        for (auto const& item : items) {
            auto const& id = item.ID();
            indexUpdateHandler(GetIndexByKey(id), id, DBOp::Add);
        }
    }
}

template<>
void ObjectDatabase<TriggerInstance>::SaveInto(CIniFile& ini, std::ostream& err)
{
    auto& triggerSec = ini.AddSection(SEC_TRIGGERS);
    auto& eventsSec = ini.AddSection(SEC_EVENTS);
    auto& actionsSec = ini.AddSection(SEC_ACTIONS);
    triggerSec.Clear();
    eventsSec.Clear();
    actionsSec.Clear();

    for (auto const& trigger : items) {
        triggerSec.SetString(trigger.ID(), trigger.Options().Serialize());
        eventsSec.SetString(trigger.ID(), trigger.Events().Serialize());
        actionsSec.SetString(trigger.ID(), trigger.Actions().Serialize());
    }
}

// -------------------------- TriggerInstance -------------------
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

void TriggerInstance::SetName(const CString& name, bool updateIndex)
{
    auto const oldName = std::exchange(Options().name, name);
    if (updateIndex && oldName != name) {
        DB::Triggers.OnUpdateIndex(oldName, id, DBOp::Delete);
        DB::Triggers.OnUpdateIndex(name, id, DBOp::Add);
    }
}

// ---------------------------- TagDatabase --------------------------
template<>
void ObjectDatabase<TagInstance>::LoadFrom(const CIniFile& ini, std::ostream& err)
{
    Clear();
    auto const& tagSec = ini[SEC_TAGS];
    items.reserve(tagSec.Size());

    for (auto const& [id, opts] : tagSec) {
        items.emplace_back(id, tagSec.GetString(id));
        lookupTable.try_emplace(id, items.size() - 1);
    }
}

template<>
void ObjectDatabase<TagInstance>::SaveInto(CIniFile& ini, std::ostream& err)
{
    auto& tagSec = ini.AddSection(SEC_TAGS);
    tagSec.Clear();

    for (auto const& tag : items) {
        tagSec.SetString(tag.id, tag.Serialize());
    }
}

// This is a name - id reverse mapping
// e.g.
// - Print String (0000000004)
// - Print String (0000000005)
// - Team A Ready To Move (0000000006)
// - Team B Ready To Move (0000000002)
// so the Idx here is Trigger's name, while ID is the primK
void TriggerDatabase::OnUpdateIndex(const CString& idx, const CString& primK, const DBOp op)
{
    if (op == DBOp::Add) {
        customIndexTable.emplace(idx, primK);
        return;
    }
    if (op == DBOp::Delete) {
        auto [it, last] = customIndexTable.equal_range(idx);
        for (; it != last;) {
            if (it->second == primK) {
                it = customIndexTable.erase(it);
                break; // no duplicated elements allowed right ?
            }
            ++it;
        }
    }
    if (op == DBOp::DeleteAll) {
        customIndexTable.clear();
    }
}
