#pragma once
#include <future>
#include <atomic>
#include "velopack.hpp"

class CUpdate
{
public:
    static CUpdate& Instance();

    void PrepareUpdate();
    void CheckUpdateAsync();
    void ExecuteUpdateNow();
    bool IsUpdateRequired()
    {
        return UpdateRequired;
    }

private:
    CUpdate();

    Velopack::UpdateManager UpdateMgr;
    std::optional<Velopack::UpdateInfo> UpdateInfo;
    std::atomic_bool UpdateRequired;
    std::future<void> CheckJob;
};
