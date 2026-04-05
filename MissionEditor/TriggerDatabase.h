#pragma once
#include <concepts>
#include <type_traits>
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

    void SetName(const CString& name, bool updateIndex);

private:
    CString id;
    TriggerOptions options;
    TriggerEvents events;
    TriggerActions actions;
};

enum class DBOp
{
    Add,
    Delete,
};

// primK points to unique primary key
// while idx is the current focusing index
using OnIndexUpdate = std::function<void(const CString& idx, const CString& primK, const DBOp op)>;

template<typename TObject>
class ObjectDatabase
{
public:
    using ObjectType = TObject;

    auto const Size() const { return items.size(); }
    auto& Nth(size_t slot) {
        return items.at(slot);
    }
    auto const& Nth(size_t slot) const {
        return items.at(slot);
    }
    TObject& Lookup(const CString& id);
    TObject* TryLookup(const CString& id);
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

    void SetIndexUpdateHandler(OnIndexUpdate&& handler)
    {
        indexUpdateHandler = std::move(handler);
    }
    virtual const CString GetIndexByKey(const CString& key) const
    {
        char buffer[0x40];
        sprintf_s(buffer, "%s GetIndexByKey not implemented", typeid(this).name());
        throw std::runtime_error(buffer);
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

protected:
    OnIndexUpdate indexUpdateHandler;
    std::vector<TObject> items;
    std::map<CString, size_t> lookupTable; // ID - index of items
};

template<typename TObject>
TObject& ObjectDatabase<TObject>::Lookup(const CString& id)
{
    auto const ptr = TryLookup(id);
    if (ptr) {
        return *ptr;
    }
    throw std::runtime_error("no such trigger");
}

// implement TryLookUp
template<typename TObject>
TObject* ObjectDatabase<TObject>::TryLookup(const CString& id)
{
    auto const it = lookupTable.find(id);
    if (it != lookupTable.end()) {
        return { &items.at(it->second) };
    }
    return nullptr;
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
    if (indexUpdateHandler) {
        indexUpdateHandler(this->GetIndexByKey(key), key, DBOp::Add);
    }
    return items.at(idx);
}

template<typename TObject>
void ObjectDatabase<TObject>::Append(TObject&& inst)
{
    auto const [it, _ ] = lookupTable.insert_or_assign(inst.ID(), items.size());
    items.emplace_back(std::move(inst));
    if (indexUpdateHandler) {
        indexUpdateHandler(this->GetIndexByKey(it->first), it->first, DBOp::Add);
    }
}

template<typename TObject>
TObject& ObjectDatabase<TObject>::Append(const CString& id, CString&& name)
{
    auto const [it, _] = lookupTable.insert_or_assign(id, items.size());
    auto& ret = items.emplace_back(id);
    ret.SetName(name, true); // index update happens inside
    //if (indexUpdateHandler) {
    //    indexUpdateHandler(this->GetIndexByKey(it->first), it->first, DBOp::Add);
    //}
    return ret;
}

template<typename TObject>
void ObjectDatabase<TObject>::DeleteAt(size_t idx)
{
    ASSERT(idx < items.size());
    // delete from record first;
    auto const& trigger = items.at(idx);
    if (indexUpdateHandler) {
        indexUpdateHandler(this->GetIndexByKey(trigger.ID()), trigger.ID(), DBOp::Delete);
    }
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
    using NameIndexMap = std::multimap<CString, CString>;
public:
    TriggerDatabase() : ObjectDatabase<TriggerInstance>()
    { 
        this->SetIndexUpdateHandler([this](const CString& idx, const CString& primK, const DBOp op) {
            this->OnUpdateIndex(idx, primK, op);
        });
    }

    const NameIndexMap& CustomIndex() const
    {
        return customIndexTable;
    }
    virtual const CString GetIndexByKey(const CString& key) const override
    {
        return this->Lookup(key).Options().Name();
    }

    void OnUpdateIndex(const CString& idx, const CString& primK, const DBOp op);
private:

    NameIndexMap customIndexTable; // anything mapping to ID. 
    // Will consider refactor to several slots vector, for multiple customized index tables
};

class TagDatabase : public ObjectDatabase<TagInstance>
{

};

template<typename T>
concept is_database_type = requires {
    typename T::ObjectType;
} && std::derived_from<T, ObjectDatabase<typename T::ObjectType>>;


class DatabaseManager {
public:
    template<typename T>
        requires is_database_type<T>
    static inline T& Instance = []() -> T& {
        static T instance;
        return instance;
    }();
};

namespace DB {
    inline auto& Triggers = DatabaseManager::Instance<TriggerDatabase>;
    inline auto& Tags = DatabaseManager::Instance<TagDatabase>;
}
