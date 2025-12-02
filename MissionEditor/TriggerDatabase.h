#pragma once
#include "TriggerDef.h"
#include "IniFile.h"

class TagInstance
{
public:

private:
};

class TriggerOptions
{
public:
    CString Serialize() const;


private:
};

class TriggerInstance
{
public:
    TriggerInstance(const CString& id);
    TriggerInstance(const CString& id, const CIniFile& ini);

    const CString& ID() const { return id; }

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

    TriggerInstance& Lookup(const CString& id);
    TriggerInstance& InsertAt(size_t slot, CString&& id = {});

    void LoadFrom(const CIniFile& ini, std::ostream& err);
    void SaveInto(CIniFile& ini, std::ostream& err);

    TriggerDatabase() = default;
    TriggerDatabase(const TriggerDatabase&) = delete;

private:
    std::vector<TriggerInstance> items;
    std::map<CString, size_t> lookupTable; // ID - index of items
    // TODO: consider tags
};
