#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"

BEGIN_MESSAGE_MAP(CTriggerEditorAllDlg, CDialog)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDD_TRGR_CLONE_TRIGGER, &CTriggerEditorAllDlg::OnBnClickedTrgrCloneTrigger)
    ON_BN_CLICKED(IDD_TRGR_DISABLED, OnDisabled)
    ON_BN_CLICKED(IDD_TRGR_EASY, OnEasy)
    ON_BN_CLICKED(IDD_TRGR_MEDIUM, OnMedium)
    ON_BN_CLICKED(IDD_TRGR_HARD, OnHard)
    ON_CBN_EDITCHANGE(IDD_TRGR_SELECTED_TRIGGER, onEditChangeTriggerType)
END_MESSAGE_MAP()

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

    DDX_Control(pDX, IDD_TRGR_SELECTED_TRIGGER, m_triggerType);
    DDX_Control(pDX, IDD_TRGR_NAME, m_triggerName);
    DDX_Control(pDX, IDD_TRGR_HOUSE, m_house);
    DDX_Control(pDX, IDD_TRGR_TYPE, m_persistence);
    DDX_Control(pDX, IDD_TRGR_ATTACHED_TRIGGER, m_nextTrigger);
    DDX_Control(pDX, IDD_TRGR_MEDIUM, m_medium);
    DDX_Control(pDX, IDD_TRGR_HARD, m_hard);
    DDX_Control(pDX, IDD_TRGR_EASY, m_easy);
    DDX_Control(pDX, IDD_TRGR_DISABLED, m_disabled);

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

BOOL CTriggerEditorAllDlg::PreTranslateMessage(MSG* pMsg)
{
    return CDialog::PreTranslateMessage(pMsg);
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

    CIniFile& ini = Map->GetIniFile();
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    ListHouses(m_house, FALSE, TRUE, FALSE);
    ListTriggers(m_nextTrigger);
    m_nextTrigger.InsertString(0, "<none>");

    auto triggerCopy = ini["Triggers"][m_currentTrigger];
    if (RepairTrigger(triggerCopy)) {
        ini.SetString("Triggers", m_currentTrigger, triggerCopy);
    }

    m_triggerName.SetWindowText(GetParam(triggerCopy, 2));
    m_house.SetWindowText(TranslateHouse(GetParam(triggerCopy, 0), TRUE));
    CString attachedTrigger = GetParam(triggerCopy, 1);
    m_nextTrigger.SetWindowText(attachedTrigger);

    m_disabled.SetCheck((atoi(GetParam(triggerCopy, 3))));
    m_easy.SetCheck((atoi(GetParam(triggerCopy, 4))));
    m_medium.SetCheck((atoi(GetParam(triggerCopy, 5))));
    m_hard.SetCheck((atoi(GetParam(triggerCopy, 6))));

    for (auto i = 0; i < m_nextTrigger.GetCount(); i++) {
        CString tmp;
        m_nextTrigger.GetLBText(i, tmp);
        TruncSpace(tmp);
        if (tmp == attachedTrigger) {
            m_nextTrigger.SetCurSel(i);
        }
    }

    for (auto const& [type, def] : ini["Tags"]) {
        CString attTrigg = GetParam(def, 2);
        if (attTrigg == m_currentTrigger) {
            m_triggerType.SetWindowText(GetParam(def, 0));
            break;
        }
    }
}



void CTriggerEditorAllDlg::onChangeTriggerName()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CIniFile& ini = Map->GetIniFile();

    if (!ini["Triggers"].Exists(m_currentTrigger)) {
        return;
    }

    CString newName;
    m_triggerName.GetWindowText(newName);

    if (newName.GetLength() == 0) {
        newName = " ";
    }

    if (newName.Find(",", 0) >= 0) {//newName.SetAt(newName.Find(",",0), 0);
        newName = newName.Left(newName.Find(",", 0));

        m_triggerName.SetWindowText(newName);
    }

    ini.SetString("Triggers", m_currentTrigger, SetParam(ini["Triggers"][m_currentTrigger], 2, newName));

    int i;
    int p = 0;
    for (auto const& [type, def] : ini["Tags"]) {
        CString attTrigg = GetParam(def, 2);
        if (attTrigg == m_currentTrigger) {
            p++;
            char c[50];
            itoa(p, c, 10);
            CString newVal = newName + " ";
            newVal += c;
            ini.SetString("Tags", type, SetParam(ini.GetString("Tags", type), 1, newVal));
        }
    }
    //MessageBox(ini.sections["Triggers"].values[m_currentTrigger],newName);
    auto triggerCopy = ini["Triggers"][m_currentTrigger];
    if (RepairTrigger(triggerCopy)) {
        ini.SetString("Triggers", m_currentTrigger, triggerCopy);
    }
    onKillFocusName();
}

void CTriggerEditorAllDlg::onEditChangeHouse()
{
    CIniFile& ini = Map->GetIniFile();

    if (!ini["Triggers"].Exists(m_currentTrigger) || m_currentTrigger.IsEmpty()) {
        return;
    }

    CString newHouse;
    m_house.GetWindowText(newHouse);

    newHouse = TranslateHouse(newHouse);

    newHouse.TrimLeft();
    TruncSpace(newHouse);
    if (newHouse.Find(",", 0) >= 0) {
        newHouse.SetAt(newHouse.Find(",", 0), 0);
    }

    ini.SetString("Triggers", m_currentTrigger, SetParam(ini["Triggers"][m_currentTrigger], 0, newHouse));

    auto triggerCopy = ini["Triggers"][m_currentTrigger];
    if (RepairTrigger(triggerCopy)) {
        ini.SetString("Triggers", m_currentTrigger, triggerCopy);
    }
}

void CTriggerEditorAllDlg::onEditChangeNextTrigger()
{
    CIniFile& ini = Map->GetIniFile();

    if (!ini["Triggers"].Exists(m_currentTrigger) || m_currentTrigger.IsEmpty()) {
        return;
    }

    CString newTrigger;
    m_nextTrigger.GetWindowText(newTrigger);
    newTrigger.TrimLeft();
    TruncSpace(newTrigger);

    if (newTrigger.Find(",", 0) >= 0) {
        newTrigger.SetAt(newTrigger.Find(",", 0), 0);
    }

    ini.SetString("Triggers", m_currentTrigger, SetParam(ini["Triggers"][m_currentTrigger], 1, newTrigger));
}

void CTriggerEditorAllDlg::onKillFocusName()
{
    UpdateDialog();
}

void CTriggerEditorAllDlg::onKillFocus(CWnd* pNewWnd)
{
    CDialog::OnKillFocus(pNewWnd);
    UpdateDialog();
}

void CTriggerEditorAllDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (!bShow) {
        if (auto pOldFocus = GetFocus()) {
            switch (pOldFocus->GetDlgCtrlID())
            {
            case IDD_TRGR_NAME: {
                this->onChangeTriggerName();
            } break;
            default:
                break;
            }
        }
    }
    CDialog::OnShowWindow(bShow, nStatus);
}

void CTriggerEditorAllDlg::onEditChangeTriggerType()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CIniFile& ini = Map->GetIniFile();

    if (!ini["Triggers"].Exists(m_currentTrigger)) {
        return;
    }

    CString newType;
    m_triggerType.GetWindowText(newType);
    TruncSpace(newType);

    int i;
    for (auto const& [type, def] : ini["Tags"]) {
        CString attTrigg = GetParam(def, 2);
        if (attTrigg == m_currentTrigger) {
            ini.SetString("Tags", type, SetParam(ini["Tags"][type], 0, newType));
        }
    }

    auto trigger = ini["Triggers"][m_currentTrigger];
    if (RepairTrigger(trigger)) {
        ini.SetString("Triggers", m_currentTrigger, trigger);
    }
}

void CTriggerEditorAllDlg::onOptionCheckChanged(const CButton& checkBtn, const int paramPos)
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    auto& ini = Map->GetIniFile();
    auto const& triggersSec = ini["Triggers"];

    if (!triggersSec.Exists(m_currentTrigger)) {
        return;
    }
    auto const checked = checkBtn.GetCheck() != 0;
    auto const param = checked ? "1" : "0";
    auto const newVal = SetParam(triggersSec[m_currentTrigger], paramPos, param);
    ini.SetString("Triggers", m_currentTrigger, newVal);
}

void CTriggerEditorAllDlg::OnDisabled()
{
    onOptionCheckChanged(m_disabled, 3);
}

void CTriggerEditorAllDlg::OnEasy()
{
    onOptionCheckChanged(m_easy, 4);
}

void CTriggerEditorAllDlg::OnMedium()
{
    onOptionCheckChanged(m_medium, 5);
}

void CTriggerEditorAllDlg::OnHard()
{
    onOptionCheckChanged(m_hard, 6);
}

void CTriggerEditorAllDlg::OnBnClickedTrgrCloneTrigger()
{
    // TODO: Add your control notification handler code here
}
