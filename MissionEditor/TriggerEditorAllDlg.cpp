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
    Actionframe = 50934,
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
    TranslateDlgItem(*this, 50901, "TriggerTriggeroptions");
    TranslateDlgItem(*this, 50902, "TriggerSelectedTrigger");
    TranslateDlgItem(*this, NewTrigger, "TriggerNew");
    TranslateDlgItem(*this, CloneTrigger, "TriggerClone");
    TranslateDlgItem(*this, DeleteTrigger, "TriggerDelete");
    TranslateDlgItem(*this, PlaceOnMap, "TriggerPlaceOnMap");
    TranslateDlgItem(*this, 50908, "TriggerType");
    TranslateDlgItem(*this, 50910, "TriggerName");
    TranslateDlgItem(*this, 50912, "TriggerHouse");
    TranslateDlgItem(*this, 50914, "TriggerAttachedtrigger");
    TranslateDlgItem(*this, 50915, "TriggerCannotbeitselforformsaloop");
    TranslateDlgItem(*this, Disabled, "TriggerDisabled");
    TranslateDlgItem(*this, Easy, "TriggerEasy");
    TranslateDlgItem(*this, Medium, "TriggerMedium");
    TranslateDlgItem(*this, Hard, "TriggerHard");
    TranslateDlgItem(*this, 50921, "TriggerEventoptions");
    TranslateDlgItem(*this, 50922, "TriggerEventtype");
    TranslateDlgItem(*this, NewEvent, "TriggerAdd");
    TranslateDlgItem(*this, CloneEvent, "TriggerClone");
    TranslateDlgItem(*this, DeleteEvent, "TriggerDelete");
    TranslateDlgItem(*this, 50928, "TriggerEventList");
    TranslateDlgItem(*this, EventParameter1Desc, "TriggerParameter#1value");
    TranslateDlgItem(*this, EventParameter2Desc, "TriggerParameter#2value");
    TranslateDlgItem(*this, Actionoptions, "TriggerActionoptions");
    TranslateDlgItem(*this, 50935, "TriggerActiontype");
    TranslateDlgItem(*this, NewAction, "TriggerAdd");
    TranslateDlgItem(*this, DeleteAction, "TriggerDelete");
    TranslateDlgItem(*this, CloneAction, "TriggerClone");
    TranslateDlgItem(*this, 50941, "TriggerActionList");
    TranslateDlgItem(*this, ActionParameter1Desc, "TriggerParameter#1value");
    TranslateDlgItem(*this, ActionParameter2Desc, "TriggerParameter#2value");
    TranslateDlgItem(*this, ActionParameter3Desc, "TriggerParameter#3value");
    TranslateDlgItem(*this, ActionParameter4Desc, "TriggerParameter#4value");
    TranslateDlgItem(*this, ActionParameter5Desc, "TriggerParameter#5value");
    TranslateDlgItem(*this, ActionParameter6Desc, "TriggerParameter#6value");
    TranslateDlgItem(*this, SearchReference, "SearchReferenceTitle");
}

void CTriggerEditorAllDlg::UpdateDialog()
{

}
