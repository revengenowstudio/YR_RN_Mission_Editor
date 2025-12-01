#pragma once
#include "IniFile.h"

class TagInstance
{
public:

private:
};

class TriggerOptions
{
public:

private:
};

class TriggerInstance
{
public:
    TriggerInstance(const CIniFile& ini, std::ostream& err);

    CString Serialize();

private:
    TriggerOptions options;
    TriggerEvents events;
    TriggerActions actions;
};

class TriggerDatabase
{
public:
    static TriggerDatabase& Instance();

    TriggerDatabase() = default;
    TriggerDatabase(const TriggerDatabase&) = delete;

private:
    std::vector<TriggerInstance> items;
    std::unordered_map<CString, size_t, CStringHash> lookupTable; // ID - index of items
};
