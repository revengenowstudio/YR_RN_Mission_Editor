#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"

BEGIN_MESSAGE_MAP(CTriggerEditorAllDlg, CDialog)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_TRGR_NEW_TRIGGER, onNewTrigger)
    ON_BN_CLICKED(IDC_TRGR_CLONE_TRIGGER, &CTriggerEditorAllDlg::OnBnClickedTrgrCloneTrigger)
    ON_BN_CLICKED(IDC_TRGR_DISABLED, OnDisabled)
    ON_BN_CLICKED(IDC_TRGR_EASY, OnEasy)
    ON_BN_CLICKED(IDC_TRGR_MEDIUM, OnMedium)
    ON_BN_CLICKED(IDC_TRGR_HARD, OnHard)
    ON_CBN_EDITCHANGE(IDC_TRGR_SELECTED_TRIGGER, onEditChangeTriggerType)
    ON_CBN_SELCHANGE(IDC_TRGR_SELECTED_TRIGGER, onSelChangeTrigger)
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

    GetDlgItem(IDC_TRGR_SEARCH_REFERENCE)->EnableWindow(FALSE); // not yet ready

    parseTriggerDefinitions();
    oneTimeInit();

    return TRUE;
}

void CTriggerEditorAllDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_TRGR_SELECTED_TRIGGER, m_triggerType);
    DDX_Control(pDX, IDC_TRGR_NAME, m_triggerName);
    DDX_Control(pDX, IDC_TRGR_HOUSE, m_house);
    DDX_Control(pDX, IDC_TRGR_TYPE, m_persistence);
    DDX_Control(pDX, IDC_TRGR_ATTACHED_TRIGGER, m_nextTrigger);
    DDX_Control(pDX, IDC_TRGR_MEDIUM, m_medium);
    DDX_Control(pDX, IDC_TRGR_HARD, m_hard);
    DDX_Control(pDX, IDC_TRGR_EASY, m_easy);
    DDX_Control(pDX, IDC_TRGR_DISABLED, m_disabled);

    DDX_Control(pDX, IDC_TRGR_EVENT_TYPE, m_eventTypes);
    DDX_Control(pDX, IDC_TRGR_EVENT_LIST, m_eventList);
    DDX_Control(pDX, IDC_TRGR_EVENT_PARAMETER_1, m_eventParam1);
    DDX_Control(pDX, IDC_TRGR_EVENT_PARAMETER_2, m_eventParam2);
    DDX_Control(pDX, IDC_TRGR_EVENT_DESCRIPTION, m_eventDescription);

    DDX_Control(pDX, IDC_TRGR_ACTION_TYPE, m_actionTypes);
    DDX_Control(pDX, IDC_TRGR_ACTION_LIST, m_actionList);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_1, m_actionParam[0]);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_2, m_actionParam[1]);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_3, m_actionParam[2]);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_4, m_actionParam[3]);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_5, m_actionParam[4]);
    DDX_Control(pDX, IDC_TRGR_ACTION_PARAMETER_6, m_actionParam[5]);
    DDX_Control(pDX, IDC_TRGR_ACTION_DESCRIPTION, m_actionDescription);
}

BOOL CTriggerEditorAllDlg::PreTranslateMessage(MSG* pMsg)
{
    return CDialog::PreTranslateMessage(pMsg);
}

void CTriggerEditorAllDlg::translateUI()
{
    TranslateWindowCaption(*this, "TriggerCaption");

    TranslateDlgItem(*this, IDC_TRGR_TRIGGER_OPTIONS, "TriggerOptions");
    TranslateDlgItem(*this, IDC_TRGR_SELECT_TRIGGER_TXT, "TriggerDesc");
    TranslateDlgItem(*this, IDC_TRGR_NEW_TRIGGER, "TriggerNew");
    TranslateDlgItem(*this, IDC_TRGR_CLONE_TRIGGER, "TriggerClone");
    TranslateDlgItem(*this, IDC_TRGR_DELETE_TRIGGER, "TriggerDelete");
    TranslateDlgItem(*this, IDC_TRGR_PLACE_ON_MAP, "TriggerPlaceOnMap");
    TranslateDlgItem(*this, IDC_TRGR_TYPE_TXT, "TriggerOptionType");
    TranslateDlgItem(*this, IDC_TRGR_NAME_TXT, "TriggerName");
    TranslateDlgItem(*this, IDC_TRGR_HOUSE_TXT, "TriggerOptionHouse");
    TranslateDlgItem(*this, IDC_TRGR_ATTACHED_TRIGGER_TXT, "TriggerOptionAttachedTrigger");
    TranslateDlgItem(*this, IDC_TRGR_ATTACHED_TRIGGER, "TriggerCannotbeitselforformsaloop");
    TranslateDlgItem(*this, IDC_TRGR_DISABLED, "TriggerOptionDisabled");
    TranslateDlgItem(*this, IDC_TRGR_EASY, "TriggerOptionEasy");
    TranslateDlgItem(*this, IDC_TRGR_MEDIUM, "TriggerOptionMedium");
    TranslateDlgItem(*this, IDC_TRGR_HARD, "TriggerOptionHard");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_OPTIONS, "TriggerEventoptions");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_TYPE_TXT, "TriggerEventtype");
    TranslateDlgItem(*this, IDC_TRGR_NEW_EVENT, "TriggerAdd");
    TranslateDlgItem(*this, IDC_TRGR_CLONE_EVENT, "TriggerClone");
    TranslateDlgItem(*this, IDC_TRGR_DELETE_EVENT, "TriggerDelete");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_LIST_TXT, "TriggerEventList");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_P1_TXT, "TriggerParameter#1value");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_P2_TXT, "TriggerParameter#2value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_OPTIONS, "TriggerActionoptions");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_TYPE_TXT, "TriggerActiontype");
    TranslateDlgItem(*this, IDC_TRGR_NEW_ACTION, "TriggerAdd");
    TranslateDlgItem(*this, IDC_TRGR_DELETE_ACTION, "TriggerDelete");
    TranslateDlgItem(*this, IDC_TRGR_CLONE_ACTION, "TriggerClone");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_LIST_TXT, "TriggerActionList");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P1_TXT, "TriggerParameter#1value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P2_TXT, "TriggerParameter#2value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P3_TXT, "TriggerParameter#3value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P4_TXT, "TriggerParameter#4value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P5_TXT, "TriggerParameter#5value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_P6_TXT, "TriggerParameter#6value");
    TranslateDlgItem(*this, IDC_TRGR_SEARCH_REFERENCE, "SearchReferenceTitle");
}

void CTriggerEditorAllDlg::clear()
{
    while (m_triggerType.DeleteString(0) != CB_ERR);
    while (m_house.DeleteString(0) != CB_ERR);
    while (m_nextTrigger.DeleteString(0) != CB_ERR);

}

void CTriggerEditorAllDlg::oneTimeInit()
{
    m_persistence.InsertString(0, TranslateStringACP("0 - Standard"));
    m_persistence.InsertString(1, TranslateStringACP("1 - All Attached"));
    m_persistence.InsertString(2, TranslateStringACP("2 - Repeating"));

    // default disable, until enabled by action
    for (auto& paramCb : m_actionParam) {
        paramCb.EnableWindow(FALSE);
    }
}

void listTriggers(CComboBox& cb)
{
    CIniFile& ini = Map->GetIniFile();

    while (cb.DeleteString(0) != CB_ERR);
    auto const& triggersSec = ini["Triggers"];

    CString triggerDisplay;
    for (auto idx = 0; idx < triggersSec.Size(); idx++) {
        auto const& [type, params] = triggersSec.Nth(idx);
        if (params.IsEmpty()) {
            continue;
        }
        triggerDisplay.Format("%s (%s)", type, GetParam(params, 2));
        int id = cb.AddString(triggerDisplay);
        cb.SetItemData(id, idx);
    }
}

void CTriggerEditorAllDlg::updateTriggerOptions()
{
    ListHouses(m_house, FALSE, TRUE, FALSE);
    listTriggers(m_nextTrigger);
    m_nextTrigger.InsertString(0, "<none>");

    onSelChangeOption();
}

void CTriggerEditorAllDlg::updateTriggerEvents()
{
    if (m_currentTrigger.GetLength() == 0) {
        while (m_eventList.DeleteString(0) != CB_ERR);
        return;
    }

    while (m_eventTypes.DeleteString(0) != CB_ERR);

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto const& [eventid, eventdata] : defMgr.Events()) {
#ifdef RA2_MODE
        if (!eventdata.ra2Allowed) {
            continue;
        }
        if (yuri_mode && eventdata.yrOnly) {
            continue;
        }
#else
        if (!eventdata.tsAllowed) {
            continue;
        }
#endif
        m_eventTypes.AddString(eventdata.brief);
    }

    int cur_sel = m_eventList.GetCurSel();
    while (m_eventList.DeleteString(0) != CB_ERR);

    CIniFile& ini = Map->GetIniFile();
    auto const& data = ini["Events"][m_currentTrigger];
    TriggerEvents events(data);
    auto const eventCount = events.Size();

    CString eventDesc;
    for (auto i = 0; i < eventCount; i++) {
        auto const eventIdx = events.Nth(i).eventType;
        auto const& brief = defMgr.Events().at(eventIdx).brief;
        eventDesc.Format("%d %s", i, brief);

        m_eventList.SetItemData(m_eventList.AddString(eventDesc), i);
    }
    if (cur_sel < 0) {
        cur_sel = 0;
    }
    if (cur_sel >= eventCount) {
        cur_sel = eventCount - 1;
    }

    m_eventList.SetCurSel(cur_sel);

    onSelChangeEvent();
}

void CTriggerEditorAllDlg::updateTriggerActions()
{
}

void CTriggerEditorAllDlg::UpdateDialog()
{
    clear();

#if 0
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
#endif

    // means first time open, try load trigger types
    if (m_triggerType.GetCount() <= 0) {
        CIniFile& ini = Map->GetIniFile();

        listTriggers(m_triggerType);

        if (m_triggerType.GetCount() > 0) {
            m_triggerType.SetCurSel(0);
            CString firstTypeId;
            m_triggerType.GetWindowText(firstTypeId);
            TruncSpace(firstTypeId);
            m_currentTrigger = firstTypeId;
        }
    }

    updateTriggerOptions();
    updateTriggerEvents();
    updateTriggerActions();
}

void CTriggerEditorAllDlg::onNewTrigger()
{
    CIniFile& ini = Map->GetIniFile();

    CString newId = GetFreeID();
    ini.SetString("Triggers", newId, Map->GetHouseID(0, TRUE) + ",<none>,New trigger,0,1,1,1,0");
    ini.SetString("Events", newId, "0");
    ini.SetString("Actions", newId, "0");

    //if(MessageBox("Trigger created. If you want to create a simple tag now, press Yes. The tag will be called ""New tag"", you should name it like the trigger (after you have set up the trigger).","Trigger created",MB_YESNO))
    {
        CString tagId = GetFreeID();
        ini.SetString("Tags", tagId, "0,New tag," + newId);
    }

    theApp.MainWindow()->UpdateDialogs(TRUE);

    auto const& triggerSec = ini["Triggers"];
    for (auto i = 0; i < m_triggerType.GetCount(); i++) {
        if (m_triggerType.GetItemData(i) == triggerSec.FindIndex(newId)) {
            m_triggerType.SetCurSel(i);
        }
    }
    onSelChangeTrigger();
}

void CTriggerEditorAllDlg::onSelChangeTrigger()
{
    int curSel = m_triggerType.GetCurSel();
    if (curSel < 0) {
        clear();
        return;
    }
    int curInd = m_triggerType.GetItemData(curSel);
    CIniFile& ini = Map->GetIniFile();
    m_currentTrigger = ini["Triggers"].Nth(curInd).first;

    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    // update persistence value, necessary here ?
    // why don't only handle for m_persistence events ?
#if 0
    CString newType;
    m_triggerType.GetWindowText(newType);
    TruncSpace(newType);
    for (auto const& [type, def] : ini["Tags"]) {
        if (GetParam(def, 2) == m_currentTrigger) {
            ini.SetString("Tags", type, SetParam(ini["Tags"][type], 0, newType));
        }
    }
#endif

    // really ? should not be needed at all
#if 0
    auto trigger = ini["Triggers"][m_currentTrigger];
    if (RepairTrigger(trigger)) {
        ini.SetString("Triggers", m_currentTrigger, trigger);
    }
#endif
    onSelChangeOption();
    onSelChangeEvent();
    onSelChangeAction();
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
            case IDC_TRGR_NAME: {
                this->onChangeTriggerName();
            } break;
            default:
                break;
            }
        }
    }
    CDialog::OnShowWindow(bShow, nStatus);
}

// this function will work as filter
void CTriggerEditorAllDlg::onEditChangeTriggerType()
{
    
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

void CTriggerEditorAllDlg::parseTriggerDefinitions()
{
    TriggerDefinitionManager::Instance().LoadFrom(g_data, errstream);
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

void CTriggerEditorAllDlg::onSelChangeOption()
{
    CIniFile& ini = Map->GetIniFile();

    auto triggerCopy = ini["Triggers"][m_currentTrigger];
    // considering remove it
#if 0
    if (RepairTrigger(triggerCopy)) {
        ini.SetString("Triggers", m_currentTrigger, triggerCopy);
    }
#endif
    auto const triggerParams = SplitParams<7>(triggerCopy);

    m_triggerName.SetWindowText(triggerParams[2]);
    m_house.SetWindowText(TranslateHouse(triggerParams[0], TRUE));
    CString attachedTrigger = triggerParams[1];
    m_nextTrigger.SetWindowText(attachedTrigger);

    m_disabled.SetCheck((atoi(triggerParams[3])));
    m_easy.SetCheck((atoi(triggerParams[4])));
    m_medium.SetCheck((atoi(triggerParams[5])));
    m_hard.SetCheck((atoi(triggerParams[6])));

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
            // update persistence
            m_persistence.SetWindowText(GetParam(def, 0));
            break;
        }
    }
}

void CTriggerEditorAllDlg::onSelChangeEvent()
{

}

void CTriggerEditorAllDlg::onSelChangeAction()
{

}

void CTriggerEditorAllDlg::OnBnClickedTrgrCloneTrigger()
{
    // TODO: Add your control notification handler code here
}
