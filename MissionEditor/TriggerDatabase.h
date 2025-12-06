#pragma once
#include "TriggerDef.h"
#include "IniFile.h"

class TagInstance
{
public:

private:
};

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

private:
    CString id;
    TriggerOptions options;
    TriggerEvents events;
    TriggerActions actions;
};

class TriggerDatabase
{
public:
    static TriggerDatabase& Instance();

    auto const Size() const { return items.size(); }
    auto& Nth(size_t slot) {
        return items.at(slot);
    }
    auto const& Nth(size_t slot) const {
        return items.at(slot);
    }
    TriggerInstance& Lookup(const CString& id);
    const TriggerInstance& Lookup(const CString& id) const {
        using BaseType = std::remove_pointer_t<decltype(this)>;
        using NonConstType = std::remove_const_t<BaseType>;
        return const_cast<NonConstType*>(this)->Lookup(id);
    }
    TriggerInstance& InsertAt(size_t slot, CString&& id = {});
    void Append(TriggerInstance&& inst);
    TriggerInstance& Append(const CString& id, CString&& name);
    void DeleteAt(size_t slot);

    void LoadFrom(const CIniFile& ini, std::ostream& err);
    void SaveInto(CIniFile& ini, std::ostream& err);
    void Clear()
    {
        items.clear();
        lookupTable.clear();
    }

    TriggerDatabase() = default;
    TriggerDatabase(const TriggerDatabase&) = delete;

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
    std::vector<TriggerInstance> items;
    std::map<CString, size_t> lookupTable; // ID - index of items
    // TODO: consider tags
};
