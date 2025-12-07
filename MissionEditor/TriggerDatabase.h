#pragma once
#include "TriggerDef.h"
#include "IniFile.h"

class TriggerInstance
{
public:
    static const TriggerInstance Default;

    TriggerInstance(const CString& id);
    TriggerInstance(const CString& id, const CIniFile& ini);
    TriggerInstance(CString&& id, CString&& name, CString&& house);

    const CString& ID() const { return id; }
    void SetID(const CString& id) { this->id = id; }

    auto& Options() { return options; }
    auto& Events() { return events; }
    auto& Actions() { return actions; }

    auto& Options() const { return options; }
    auto& Events() const { return events; }
    auto& Actions() const { return actions; }

    void SetName(CString&& name) {
        Options().name = std::move(name);
    }

private:
    CString id;
    TriggerOptions options;
    TriggerEvents events;
    TriggerActions actions;
};

template<typename TObject>
class ObjectDatabase
{
public:
    static ObjectDatabase& Instance();

    auto const Size() const { return items.size(); }
    auto& Nth(size_t slot) {
        return items.at(slot);
    }
    auto const& Nth(size_t slot) const {
        return items.at(slot);
    }
    TObject& Lookup(const CString& id);
    const TObject& Lookup(const CString& id) const {
        using BaseType = std::remove_pointer_t<decltype(this)>;
        using NonConstType = std::remove_const_t<BaseType>;
        return const_cast<NonConstType*>(this)->Lookup(id);
    }
    TObject& InsertAt(size_t slot, CString&& id = {});
    void Append(TObject&& inst);
    TObject& Append(const CString& id, CString&& name);
    void DeleteAt(size_t slot);
    bool DeleteByID(const CString& id);

    void LoadFrom(const CIniFile& ini, std::ostream& err);
    void SaveInto(CIniFile& ini, std::ostream& err);
    void Clear()
    {
        items.clear();
        lookupTable.clear();
    }

    ObjectDatabase() = default;
    ObjectDatabase(const ObjectDatabase&) = delete;

    int64_t FindIndex(const CString& key) const noexcept
    {
        auto const it = this->lookupTable.find(key);
        if (it != this->lookupTable.end()) {
            return it->second;
        }
        return -1;
    }

    bool Exists(const CString& key) const noexcept
    {
        return FindIndex(key) >= 0;
    }

    auto begin() noexcept
    {
        return items.begin();
    }

    auto begin() const noexcept
    {
        return items.begin();
    }

    auto end() noexcept
    {
        return items.end();
    }

    auto end() const noexcept
    {
        return items.end();
    }

private:
    std::vector<TObject> items;
    std::map<CString, size_t> lookupTable; // ID - index of items
};

template<typename TObject>
TObject& ObjectDatabase<TObject>::Lookup(const CString& id)
{
    auto const it = lookupTable.find(id);
    if (it != lookupTable.end()) {
        return items.at(it->second);
    }
    throw std::runtime_error("no such trigger");
}

template<typename TObject>
TObject& ObjectDatabase<TObject>::InsertAt(size_t idx, CString&& key)
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

template<typename TObject>
void ObjectDatabase<TObject>::Append(TObject&& inst)
{
    lookupTable.insert_or_assign(inst.ID(), items.size());
    items.emplace_back(std::move(inst));
}

template<typename TObject>
TObject& ObjectDatabase<TObject>::Append(const CString& id, CString&& name)
{
    lookupTable.insert_or_assign(id, items.size());
    auto& ret = items.emplace_back(id);
    ret.SetName(std::move(name));
    return ret;
}

template<typename TObject>
void ObjectDatabase<TObject>::DeleteAt(size_t idx)
{
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

template<typename TObject>
inline bool ObjectDatabase<TObject>::DeleteByID(const CString& id)
{
    auto const idx = this->FindIndex(id);
    if (idx >= 0) {
        DeleteAt(idx);
        return true;
    }
    return false;
}

class TriggerDatabase : public ObjectDatabase<TriggerInstance>
{

};

class TagDatabase : public ObjectDatabase<TagInstance>
{

};