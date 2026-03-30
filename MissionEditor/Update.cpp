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
    UpdateMgr(),
    UpdateRequired()
{ }

bool CUpdate::checkSingleNode(const std::string& url)
{
    try {
        errstream << "checking update from " << url;
        UpdateMgr = std::make_unique<Velopack::UpdateManager>(url);
        this->UpdateInfo = UpdateMgr->CheckForUpdates();
        if (!this->UpdateInfo.has_value()) {
            UpdateRequired = false;
            errstream << "no updates available";
            return true; // explicitly tells no update
        }

        // download the update, optionally providing progress callbacks
        UpdateMgr->DownloadUpdates(this->UpdateInfo.value());

        errstream << "update downloaded, notifying main window";
        ::PostMessage(theApp.MainWindow()->GetSafeHwnd(), WM_UPDATE_CHECK_FINISHED, 0, 0);

        UpdateRequired = true;
        return true;
    }
    catch (std::exception& err) {
        errstream << "update check failed, reason: " << err.what();
    }
    return false;
}

void CUpdate::PrepareUpdate()
{
    static const std::string nodes[] = {
        "https://cnb.cool/RN-Auxiliary/FinalRevenge-Releases/-/git/raw/main/",
        "https://github.com/revengenowstudio/FinalRevenge-Releases/raw/refs/heads/main/",
        "https://github.com/revengenowstudio/YR_RN_Mission_Editor/releases/latest/download/",
    };

    for (auto const& url : nodes) {
        if (checkSingleNode(url)) {
            break;
        }
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
    UpdateMgr->WaitExitThenApplyUpdates(UpdateInfo.value());
    //exit(0); // exit the app to apply the update
}
