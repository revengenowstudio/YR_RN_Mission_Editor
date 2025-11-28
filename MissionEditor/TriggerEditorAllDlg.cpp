#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"

enum Controls {
    SelectedTrigger = 50903,
    NewTrigger = 50904,
    CloneTrigger = 50905,
    DeleteTrigger = 50906,
    PlaceOnMap = 50907,
    Type = 50909,
    Name = 50911,
    House = 50913,
    Attachedtrigger = 50916,
    Disabled = 50917,
    Easy = 50918,
    Medium = 50919,
    Hard = 50920,
    Eventtype = 50923,
    NewEvent = 50924,
    CloneEvent = 50925,
    DeleteEvent = 50926,
    EventDescription = 50927,
    EventList = 50929,
    EventParameter1Desc = 50930,
    EventParameter1 = 50931,
    EventParameter2Desc = 50932,
    EventParameter2 = 50933,
    Actionoptions = 50934,
    Actiontype = 50940,
    NewAction = 50937,
    DeleteAction = 50938,
    CloneAction = 50939,
    ActionDescription = 50936,
    ActionList = 50942,
    ActionParameter1Desc = 50943,
    ActionParameter1 = 50944,
    ActionParameter2Desc = 50945,
    ActionParameter2 = 50946,
    ActionParameter3Desc = 50947,
    ActionParameter3 = 50948,
    ActionParameter4Desc = 50949,
    ActionParameter4 = 50950,
    ActionParameter5Desc = 50951,
    ActionParameter5 = 50952,
    ActionParameter6Desc = 50953,
    ActionParameter6 = 50954,
    SearchReference = 1999
};

CTriggerEditorAllDlg::CTriggerEditorAllDlg(CWnd* pParent) : 
    CDialog(IDD, pParent)
{
}

BOOL CTriggerEditorAllDlg::OnInitDialog()
{
    if (!CDialog::OnInitDialog()) {
        return FALSE;
    }

    translateUI();

    return TRUE;
}

void CTriggerEditorAllDlg::translateUI()
{
    TranslateDlgItem(*this, IDD_TRGR_TRIGGER_OPTIONS, "TriggerTriggeroptions");
    TranslateDlgItem(*this, IDD_TRGR_SELECT_TRIGGER_TXT, "TriggerSelectedTrigger");
    TranslateDlgItem(*this, IDD_TRGR_NEW_TRIGGER, "TriggerNew");
    TranslateDlgItem(*this, IDD_TRGR_CLONE_TRIGGER, "TriggerClone");
    TranslateDlgItem(*this, IDD_TRGR_DELETE_TRIGGER, "TriggerDelete");
    TranslateDlgItem(*this, IDD_TRGR_PLACE_ON_MAP, "TriggerPlaceOnMap");
    TranslateDlgItem(*this, IDD_TRGR_TYPE_TXT, "TriggerType");
    TranslateDlgItem(*this, IDD_TRGR_NAME_TXT, "TriggerName");
    TranslateDlgItem(*this, IDD_TRGR_HOUSE_TXT, "TriggerHouse");
    TranslateDlgItem(*this, IDD_TRGR_ATTACHED_TRIGGER_TXT, "TriggerAttachedtrigger");
    TranslateDlgItem(*this, IDD_TRGR_ATTACHED_TRIGGER, "TriggerCannotbeitselforformsaloop");
    TranslateDlgItem(*this, IDD_TRGR_DISABLED, "TriggerDisabled");
    TranslateDlgItem(*this, IDD_TRGR_EASY, "TriggerEasy");
    TranslateDlgItem(*this, IDD_TRGR_MEDIUM, "TriggerMedium");
    TranslateDlgItem(*this, IDD_TRGR_HARD, "TriggerHard");
    TranslateDlgItem(*this, IDD_TRGR_EVENT_OPTIONS, "TriggerEventoptions");
    TranslateDlgItem(*this, IDD_TRGR_EVENT_TYPE_TXT, "TriggerEventtype");
    TranslateDlgItem(*this, IDD_TRGR_NEW_EVENT, "TriggerAdd");
    TranslateDlgItem(*this, IDD_TRGR_CLONE_EVENT, "TriggerClone");
    TranslateDlgItem(*this, IDD_TRGR_DELETE_EVENT, "TriggerDelete");
    TranslateDlgItem(*this, IDD_TRGR_EVENT_LIST_TXT, "TriggerEventList");
    TranslateDlgItem(*this, IDD_TRGR_EVENT_P1_TXT, "TriggerParameter#1value");
    TranslateDlgItem(*this, IDD_TRGR_EVENT_P2_TXT, "TriggerParameter#2value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_OPTIONS, "TriggerActionoptions");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_TYPE_TXT, "TriggerActiontype");
    TranslateDlgItem(*this, IDD_TRGR_NEW_ACTION, "TriggerAdd");
    TranslateDlgItem(*this, IDD_TRGR_DELETE_ACTION, "TriggerDelete");
    TranslateDlgItem(*this, IDD_TRGR_CLONE_ACTION, "TriggerClone");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_LIST_TXT, "TriggerActionList");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P1_TXT, "TriggerParameter#1value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P2_TXT, "TriggerParameter#2value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P3_TXT, "TriggerParameter#3value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P4_TXT, "TriggerParameter#4value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P5_TXT, "TriggerParameter#5value");
    TranslateDlgItem(*this, IDD_TRGR_ACTION_P6_TXT, "TriggerParameter#6value");
    TranslateDlgItem(*this, IDD_TRGR_SEARCH_REFERENCE, "SearchReferenceTitle");
}

void CTriggerEditorAllDlg::UpdateDialog()
{

}
