#include "StdAfx.h"
#include "Update.h"
#include "variables.h"
#include "res/resource.h"

CUpdate& CUpdate::Instance()
{
    static CUpdate inst;
    return inst;
}

CUpdate::CUpdate() :
    UpdateMgr("https://github.com/revengenowstudio/YR_RN_Mission_Editor/releases")
{ }

void CUpdate::PrepareUpdate()
{
    try {
        errstream << "checking update";
        this->UpdateInfo = UpdateMgr.CheckForUpdates();
        if (!this->UpdateInfo.has_value()) {
            UpdateRequired = false;
            errstream << "no updates available";
            return;
        }

        // download the update, optionally providing progress callbacks
        UpdateMgr.DownloadUpdates(this->UpdateInfo.value());

        errstream << "update downloaded, notifying main window";
        ::PostMessage(theApp.MainWindow()->GetSafeHwnd(), WM_UPDATE_CHECK_FINISHED, 0, 0);

        UpdateRequired = true;
    } catch (std::exception& err) {
        errstream << "update check failed, reason: " << err.what();
    }
}

void CUpdate::CheckUpdateAsync()
{
    if (!CheckJob.valid()) {
        CheckJob = std::async(std::launch::async, [this] {
            this->PrepareUpdate();
        });
        return;
    }

    std::future_status status = CheckJob.wait_for(std::chrono::seconds(0));
    if (status != std::future_status::ready) {
        errstream << "update check in progress, skip new request";
        return;
    }
}

void CUpdate::ExecuteUpdateNow()
{
    CheckJob.wait();

    // prepare the Updater in a new process, and wait 60 seconds for this process to exit
    UpdateMgr.WaitExitThenApplyUpdates(UpdateInfo.value());
    //exit(0); // exit the app to apply the update
}
