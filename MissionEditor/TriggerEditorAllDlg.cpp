#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"


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

    GetDlgItem(IDD_TRGR_SEARCH_REFERENCE)->EnableWindow(FALSE); // not yet ready

    return TRUE;
}

void CTriggerEditorAllDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDD_TRGR_SELECTED_TRIGGER, m_selectedTrigger);
    DDX_Control(pDX, IDD_TRGR_NAME, m_triggerName);
    DDX_Control(pDX, IDD_TRGR_HOUSE, m_house);
    DDX_Control(pDX, IDD_TRGR_TYPE, m_persistence);
    DDX_Control(pDX, IDD_TRGR_ATTACHED_TRIGGER, m_nextTrigger);

    DDX_Control(pDX, IDD_TRGR_EVENT_TYPE, m_eventTypes);
    DDX_Control(pDX, IDD_TRGR_EVENT_LIST, m_eventList);
    DDX_Control(pDX, IDD_TRGR_EVENT_PARAMETER_1, m_eventParam1);
    DDX_Control(pDX, IDD_TRGR_EVENT_PARAMETER_2, m_eventParam2);
    DDX_Control(pDX, IDD_TRGR_EVENT_DESCRIPTION, m_eventDescription);

    DDX_Control(pDX, IDD_TRGR_ACTION_TYPE, m_actionTypes);
    DDX_Control(pDX, IDD_TRGR_ACTION_LIST, m_actionList);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_1, m_eventParam[0]);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_2, m_eventParam[1]);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_3, m_eventParam[2]);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_4, m_eventParam[3]);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_5, m_eventParam[4]);
    DDX_Control(pDX, IDD_TRGR_ACTION_PARAMETER_6, m_eventParam[5]);
    DDX_Control(pDX, IDD_TRGR_ACTION_DESCRIPTION, m_actionDescription);
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

void CTriggerEditorAllDlg::clear()
{

}

void CTriggerEditorAllDlg::UpdateDialog()
{
    clear();


}


