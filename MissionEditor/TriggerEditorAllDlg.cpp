#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"

extern ACTIONDATA AD; // very ugly implementation, will be refactored

BEGIN_MESSAGE_MAP(CTriggerEditorAllDlg, CDialog)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_TRGR_NEW_TRIGGER, onNewTrigger)
    ON_BN_CLICKED(IDC_TRGR_DELETE_TRIGGER, onDeleteTrigger)
    ON_BN_CLICKED(IDC_TRGR_CLONE_TRIGGER, onCloneTrigger)
    ON_BN_CLICKED(IDC_TRGR_PLACE_ON_MAP, onPlaceOnMap)
    ON_BN_CLICKED(IDC_TRGR_DISABLED, OnDisabled)
    ON_BN_CLICKED(IDC_TRGR_EASY, OnEasy)
    ON_BN_CLICKED(IDC_TRGR_MEDIUM, OnMedium)
    ON_BN_CLICKED(IDC_TRGR_HARD, OnHard)
    ON_EN_KILLFOCUS(IDC_TRGR_NAME, onChangeTriggerName)
    ON_CBN_EDITCHANGE(IDC_TRGR_SELECTED_TRIGGER, onEditChangeTriggerType)
    ON_CBN_SELCHANGE(IDC_TRGR_SELECTED_TRIGGER, onSelChangeTrigger)
    // events
    ON_LBN_SELCHANGE(IDC_TRGR_EVENT_LIST, onSelChangeEvent)
    ON_CBN_EDITCHANGE(IDC_TRGR_EVENT_TYPE, onEditChangeEventType)
    ON_CBN_EDITCHANGE(IDC_TRGR_EVENT_PARAMETER_1, onEditChangeEventValue1)
    ON_CBN_EDITCHANGE(IDC_TRGR_EVENT_PARAMETER_2, onEditChangeEventValue2)
    ON_BN_CLICKED(IDC_TRGR_NEW_EVENT, onNewEvent)
    ON_BN_CLICKED(IDC_TRGR_DELETE_EVENT, onDeleteEvent)
    ON_BN_CLICKED(IDC_TRGR_CLONE_EVENT, onCloneEvent)
    // actions
    ON_CBN_SELCHANGE(IDC_TRGR_ACTION_LIST, onSelChangeAction)
    ON_CBN_EDITCHANGE(IDC_TRGR_ACTION_TYPE, onEditChangeActionType)
    ON_CBN_EDITCHANGE(IDC_TRGR_ACTION_PARAMETER_1, onEditChangeActionValue1)
    ON_CBN_EDITCHANGE(IDC_TRGR_ACTION_PARAMETER_2, onEditChangeActionValue2)
    ON_CBN_EDITCHANGE(IDC_TRGR_ACTION_PARAMETER_3, onEditChangeActionValue3)
    ON_CBN_EDITCHANGE(IDC_TRGR_ACTION_PARAMETER_4, onEditChangeActionValue4)
    ON_CBN_DROPDOWN(IDC_TRGR_ACTION_PARAMETER_1, onDropDownActionValue1)
    ON_BN_CLICKED(IDC_TRGR_NEW_ACTION, onNewAction)
    ON_BN_CLICKED(IDC_TRGR_DELETE_ACTION, onDeleteAction)
    ON_BN_CLICKED(IDC_TRGR_CLONE_ACTION, onCloneAction)
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
    int ret = -1;
    if (pMsg->message == WM_KEYDOWN) {
        ret = onMessageKeyDown(pMsg);
    }
    return ret < 0 ? this->CDialog::PreTranslateMessage(pMsg) : ret;
}

BOOL CTriggerEditorAllDlg::onMessageKeyDown(MSG* pMsg)
{
    switch (pMsg->wParam) {
        default:
            return -1;
        case VK_RETURN:
        {
            switch (::GetDlgCtrlID(pMsg->hwnd)) {
            default:
                break;// never exist window (default -1) even nothing did
            case IDC_TRGR_NAME: this->onChangeTriggerName();
                break;
            }
        }
    }
    return TRUE;
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
    TranslateDlgItem(*this, IDC_TRGR_EVENT_OPTIONS, "TriggerEventOptions");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_TYPE_TXT, "TriggerEventType");
    TranslateDlgItem(*this, IDC_TRGR_NEW_EVENT, "TriggerNew");
    TranslateDlgItem(*this, IDC_TRGR_CLONE_EVENT, "TriggerClone");
    TranslateDlgItem(*this, IDC_TRGR_DELETE_EVENT, "TriggerDelete");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_LIST_TXT, "TriggerEventList");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_P1_TXT, "TriggerParameter#1value");
    TranslateDlgItem(*this, IDC_TRGR_EVENT_P2_TXT, "TriggerParameter#2value");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_OPTIONS, "TriggerActionoptions");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_TYPE_TXT, "TriggerActiontype");
    TranslateDlgItem(*this, IDC_TRGR_NEW_ACTION, "TriggerNew");
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
    while (m_eventList.DeleteString(0) != CB_ERR);
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

    m_actionParamTexts[0] = GetDlgItem(IDC_TRGR_ACTION_P1_TXT);
    m_actionParamTexts[1] = GetDlgItem(IDC_TRGR_ACTION_P2_TXT);
    m_actionParamTexts[2] = GetDlgItem(IDC_TRGR_ACTION_P3_TXT);
    m_actionParamTexts[3] = GetDlgItem(IDC_TRGR_ACTION_P4_TXT);
    m_actionParamTexts[4] = GetDlgItem(IDC_TRGR_ACTION_P5_TXT);
    m_actionParamTexts[5] = GetDlgItem(IDC_TRGR_ACTION_P6_TXT);
    // disable, not used
    m_actionParamTexts[4]->ShowWindow(FALSE);
    m_actionParamTexts[5]->ShowWindow(FALSE);

    // event type never changes, right ?
    while (m_eventTypes.DeleteString(0) != CB_ERR);

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto const& [eventid, eventdata] : defMgr.Events()) {
#ifdef RA2_MODE
        if (!eventdata.ra2Allowed) {
            continue;
        }
        if (!yuri_mode && eventdata.yrOnly) {
            continue;
        }
#else
        if (!eventdata.tsAllowed) {
            continue;
        }
#endif
        // TODO: use insert string and setItem data to speed up querying
        m_eventTypes.AddString(eventdata.brief);
    }

    // now handles for actionTypes
    while (m_actionTypes.DeleteString(0) != CB_ERR);
    for (auto const& [actionId, actionDef] : defMgr.Actions()) {
#ifdef RA2_MODE
        if (!actionDef.ra2Allowed) {
            continue;
        }
        if (!yuri_mode && actionDef.yrOnly) {
            continue;
        }
#else
        if (!actionDef.tsAllowed) {
            continue;
        }
#endif
        // TODO: use insert string and setItem data to speed up querying
        m_actionTypes.AddString(actionDef.brief);
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

// TODO: change name to make it adaptive for both event and action
CString makeEventShortDesc(int idx, const CString& brief)
{
    CString eventDesc;
    eventDesc.Format("%d %s", idx, brief);
    return eventDesc;
}

bool IsWaypointFormat(CString s)
{
    if (s.IsEmpty()) {
        return true;
    }

    return s[0] >= 'A' && s[0] <= 'Z';
}

void CTriggerEditorAllDlg::updateTriggerEvents()
{
    // actually only happens if no trigger at all
    if (m_currentTrigger.IsEmpty()) {
        while (m_eventList.DeleteString(0) != CB_ERR);
        return;
    }

    int cur_sel = m_eventList.GetCurSel();
    while (m_eventList.DeleteString(0) != CB_ERR);

    CIniFile& ini = Map->GetIniFile();
    auto const& data = ini["Events"][m_currentTrigger];
    TriggerEvents events(data);
    auto const eventCount = events.Size();

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto i = 0; i < eventCount; i++) {
        auto const eventIdx = events.Nth(i).eventType;
        auto const& brief = defMgr.Events().at(eventIdx).brief;
        // NOTE: maybe this event list can be simplified to use add string only
        // since events are consistent
        auto const id = m_eventList.AddString(makeEventShortDesc(i, brief));
        m_eventList.SetItemData(id, i);
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
    // actually only happens if no trigger at all
    if (m_currentTrigger.IsEmpty()) {
        while (m_actionList.DeleteString(0) != CB_ERR);
        return;
    }

    int cur_sel = m_actionList.GetCurSel();
    while (m_actionList.DeleteString(0) != CB_ERR);

    CIniFile& ini = Map->GetIniFile();
    auto const& data = ini["Actions"][m_currentTrigger];
    TriggerActions actions(data, triggerWpFilterFunc);
    auto const actionCount = actions.Size();

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto i = 0; i < actionCount; i++) {
        auto const actionIdx = actions.Nth(i).actionType;
        auto const& brief = defMgr.Actions().at(actionIdx).brief;
        // NOTE: maybe this action list can be simplified to use add string only
        // since action are consistent
        auto const id = m_actionList.AddString(makeEventShortDesc(i, brief));
        m_actionList.SetItemData(id, i);
    }
    if (cur_sel < 0) {
        cur_sel = 0;
    }
    if (cur_sel >= actionCount) {
        cur_sel = actionCount - 1;
    }

    m_actionList.SetCurSel(cur_sel);

    onSelChangeAction();
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

void CTriggerEditorAllDlg::onCloneTrigger()
{
    CIniFile& ini = Map->GetIniFile();

    int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }
    int curtrig = m_triggerType.GetItemData(sel);

    auto const triggerId = ini["Triggers"].Nth(curtrig).first;

    CString newId = GetFreeID();
    ini.SetString("Triggers", newId, ini["Triggers"][triggerId]);
    ini.SetString("Events", newId, ini["Events"][triggerId]);
    ini.SetString("Actions", newId, ini["Actions"][triggerId]);

    ini.SetString("Triggers", newId, SetParam(ini["Triggers"][newId], 2, GetParam(ini["Triggers"][newId], 2) + " Clone"));

    CString newTagId = GetFreeID();
    ini.SetString("Tags", newTagId, "0," + GetParam(ini["Triggers"][newId], 2) + "," + newId);

    theApp.MainWindow()->UpdateDialogs(TRUE);

    for (auto i = 0; i < m_triggerType.GetCount(); i++) {
        if (m_triggerType.GetItemData(i) == ini["Triggers"].FindIndex(newId)) {
            m_triggerType.SetCurSel(i);
        }
    }
    onSelChangeTrigger();
}

void CTriggerEditorAllDlg::onDeleteTrigger()
{
    int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }
    int curTrigger = m_triggerType.GetItemData(sel);
    auto const title = TranslateStringACP("Delete trigger");
    auto const content = EscapeString(TranslateStringACP("TriggerDeleteTip"));
    int res = MessageBox(content, title, MB_YESNOCANCEL);
    if (res == IDCANCEL) {
        return;
    }

    CIniFile& ini = Map->GetIniFile();
    auto const triggerId = ini["Triggers"].Nth(curTrigger).first;

    // YES means clean tags, otherwise ignore
    if (res == IDYES) {
        std::vector<CString> keysToDelete;
        for (auto const& [type, def] : ini["Tags"]) {
            auto const attTrigg = GetParam(def, 2);
            if (triggerId == attTrigg) {
                keysToDelete.push_back(type);
            }
        }
        for (auto const& keyToDelete : keysToDelete) {
            ini.RemoveValueByKey("Tags", keyToDelete);
        }
    }

    bool deleted = false;
    deleted = ini.RemoveValueByKey("Triggers", triggerId);
    ASSERT(deleted);
    deleted = ini.RemoveValueByKey("Events", triggerId);
    ASSERT(deleted);
    deleted = ini.RemoveValueByKey("Actions", triggerId);
    ASSERT(deleted);
    (void)deleted;

    theApp.MainWindow()->UpdateDialogs(TRUE);
}

void CTriggerEditorAllDlg::onPlaceOnMap()
{
    CIniFile& ini = Map->GetIniFile();

    int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }

    int curtrig = m_triggerType.GetItemData(sel);
    auto const triggerId = ini["Triggers"].Nth(curtrig).first;
    CString tag;

    for (auto const& [type, def] : ini["Tags"]) {
        CString attTrigg = GetParam(def, 2);
        if (triggerId == attTrigg) {
            tag = type;
            break;
        }
    }

    AD.mode = ACTIONMODE_CELLTAG;
    AD.type = 4;
    AD.data_s = tag;
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
    updateTriggerEvents();
    updateTriggerActions(); // update, would you?
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

    if (newName.IsEmpty()) {
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

// ========================== Trigger Events ==========================
void CTriggerEditorAllDlg::onSelChangeEvent()
{
    CIniFile& ini = Map->GetIniFile();

    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    int eventTypeSel = m_eventList.GetCurSel();
    if (eventTypeSel < 0) {
        return;
    }
    int eventIdx = m_eventList.GetItemData(eventTypeSel);

    TriggerEvents events(ini.GetString("Events", m_currentTrigger));

    auto const& eventData = events.Nth(eventIdx);
    CString eventTypeStr;
    eventTypeStr.Format("%d", eventData.eventType);

    CString tmp;
    for (auto i = 0; i < m_eventTypes.GetCount(); i++) {
        m_eventTypes.GetLBText(i, tmp);
        TruncSpace(tmp);
        if (tmp == eventTypeStr) {
            m_eventTypes.SetCurSel(i);
        }
    }

    onEditChangeEventType();
}

// TODO: support filter
void CTriggerEditorAllDlg::onEditChangeEventType()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    int eventTypeSel = m_eventList.GetCurSel();
    if (eventTypeSel < 0) {
        return;
    }

    int eventIdx = m_eventList.GetItemData(eventTypeSel);
    CString eventtype;
    m_eventTypes.GetWindowText(eventtype);
    TruncSpace(eventtype);

    // not sure necessary, maybe for new event
    if (eventtype.IsEmpty()) {
        eventtype = "0";
        m_eventTypes.SetWindowText(eventtype);
    }

    CIniFile& ini = Map->GetIniFile();
    TriggerEvents events(ini.GetString("Events", m_currentTrigger));
    auto& eventData = events.Nth(eventIdx);

    bool is4SlotEvent = false; // keep it for now, may not be necessary any more, we have safer serde
    if (eventData.param2.has_value()) {
        is4SlotEvent = true;
    }
    eventData.eventType = atoi(eventtype); // apply new event type Idx

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& eventDef = triggerDefMgr.Events().at(eventData.eventType);
    auto const& paramType1 = triggerDefMgr.Params().at(eventDef.paramTypes[0]);
    auto const& paramType2 = triggerDefMgr.Params().at(eventDef.paramTypes[1]);

    { // replace line in the list:
        m_eventList.DeleteString(eventTypeSel);
        auto const id = m_eventList.InsertString(
            eventTypeSel,
            makeEventShortDesc(eventTypeSel, eventDef.brief));
        m_eventList.SetItemData(id, eventTypeSel);
        m_eventList.SetCurSel(eventTypeSel);
    }

    if (paramType2.slotCount < 2) {// clear this slot if switching to 1 slot param
        eventData.param2.reset();
    } else if (!eventData.param2.has_value()) {// if there was no such value, give it a '0' value
        eventData.param2.emplace("0");
    }
    // otherwise, keep its value, no change

    // keep it for now. Since event type changes, param type will change accordingly 
    ini.SetString("Events", m_currentTrigger, events.Serialize());

    m_eventDescription.SetWindowText(eventDef.description);

    HandleParamList(m_eventParam1, paramType1.listType);
    HandleParamList(m_eventParam2, paramType2.listType);
    GetDlgItem(IDC_TRGR_EVENT_P1_TXT)->SetWindowTextA(paramType1.paramName);
    GetDlgItem(IDC_TRGR_EVENT_P2_TXT)->SetWindowTextA(paramType2.paramName);

    m_eventParam1.SetWindowText(eventData.param1);

    if (eventData.param2.has_value()) {
        m_eventParam2.SetWindowText(eventData.param2.value());
    }

    m_eventParam1.EnableWindow(eventDef.paramTypes[0] > 0);
    m_eventParam2.EnableWindow(eventDef.paramTypes[1] > 0);
}

void CTriggerEditorAllDlg::onEditChangeEventValue(CMyComboBox& paramCB, size_t slot)
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    if (paramCB.GetCount() > 0 && paramCB.GetCurSel() < 0) {
        paramCB.SetWindowText("");
        return;
    }

    int eventTypeSel = m_eventList.GetCurSel();
    if (eventTypeSel < 0) {
        return;
    }

    int eventIdx = m_eventList.GetItemData(eventTypeSel);

    CIniFile& ini = Map->GetIniFile();
    TriggerEvents events(ini.GetString("Events", m_currentTrigger));
    auto& eventData = events.Nth(eventIdx);

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& eventDef = triggerDefMgr.Events().at(eventData.eventType);
    auto const& paramType = triggerDefMgr.Params().at(eventDef.paramTypes[slot]);

    CString newVal;
    paramCB.GetWindowText(newVal);
    TruncSpace(newVal);
    newVal.TrimLeft();

    if (newVal.Find(",", 0) >= 0) {
        newVal.SetAt(newVal.Find(",", 0), 0);
    }

    if (slot == 0) {
        eventData.param1 = newVal;
    } else {
        eventData.param2 = newVal;
    }

    ini.SetString("Events", m_currentTrigger, events.Serialize());
}

void CTriggerEditorAllDlg::onEditChangeEventValue1()
{
    onEditChangeEventValue(m_eventParam1, 0);
}

void CTriggerEditorAllDlg::onEditChangeEventValue2()
{
    onEditChangeEventValue(m_eventParam2, 1);
}

void CTriggerEditorAllDlg::onAddEvent(TriggerEvent&& event, int slot)
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CIniFile& ini = Map->GetIniFile();
    CIniFileSection& sec = ini.AddSection("Events");
    TriggerEvents events(ini.GetString("Events", m_currentTrigger));

    events.Insert(slot, std::move(event));
    sec.SetString(m_currentTrigger, events.Serialize());

    UpdateDialog();

    m_eventList.SetCurSel(slot);
    onSelChangeEvent();
}

void CTriggerEditorAllDlg::onNewEvent()
{
    onAddEvent({
        .eventType = 0,
        .param1 = '0',
    }, 
    m_eventList.GetCount());
}

void CTriggerEditorAllDlg::onCloneEvent()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    int curEvent = m_eventList.GetCurSel();
    if (curEvent < 0) {
        return;
    }
    int eventIdx = m_eventList.GetItemData(curEvent);

    CIniFile& ini = Map->GetIniFile();
    TriggerEvents events(ini.GetString("Events", m_currentTrigger));

    onAddEvent(TriggerEvent(events.Nth(eventIdx)), curEvent + 1);
}

void CTriggerEditorAllDlg::onDeleteEvent()
{
    auto const title = TranslateStringACP("Delete event");
    auto const content = TranslateStringACP("Do you really want to delete this event?");
    if (MessageBox(content, title, MB_YESNO) == IDNO) {
        return;
    }

    CIniFile& ini = Map->GetIniFile();
    if (m_currentTrigger.GetLength() == 0) {
        return;
    }

    int curEvent = m_eventList.GetCurSel();
    if (curEvent < 0) {
        return;
    }
    int eventIdx = m_eventList.GetItemData(curEvent);
    TriggerEvents events(ini.GetString("Events", m_currentTrigger));
    events.DeleteAt(eventIdx);
    ini.SetString("Events", m_currentTrigger, events.Serialize());

    updateTriggerEvents();
    if (m_eventList.GetCount() > 0) {
        m_eventList.SetCurSel(curEvent - 1);
    }
}

// ========================== Trigger Actions ==========================
bool CTriggerEditorAllDlg::triggerWpFilterFunc(const CString& triggerCode)
{
    return g_data["DontSaveAsWP"].HasValue(triggerCode);
}

void CTriggerEditorAllDlg::onSelChangeAction()
{
    CIniFile& ini = Map->GetIniFile();

    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    int curAction = m_actionList.GetCurSel();
    if (curAction < 0) {
        return;
    }
    int actionIdx = m_actionList.GetItemData(curAction);

    TriggerActions actions(ini.GetString("Actions", m_currentTrigger), triggerWpFilterFunc);
    auto const& actionN = actions.Nth(actionIdx);

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& actionDef = triggerDefMgr.Actions().at(actionN.actionType);

    CString actionTypeStr;
    actionTypeStr.Format("%d", actionN.actionType);
    //m_actionTypes.SetWindowText(makeEventShortDesc(actionN.actionType, actionDef.brief));

    for (auto idx = 0; idx < m_actionTypes.GetCount(); idx++) {
        CString actionShortDesc;
        m_actionTypes.GetLBText(idx, actionShortDesc);
        TruncSpace(actionShortDesc);
        if (actionShortDesc == actionTypeStr) {
            m_actionTypes.SetCurSel(idx);
        }
    }

    onEditChangeActionType();
}

void CTriggerEditorAllDlg::onEditChangeActionType()
{
    if (m_currentTrigger.GetLength() == 0) {
        return;
    }
    int curAction = m_actionList.GetCurSel();
    if (curAction < 0) {
        return;
    }
    int curActionIdx = m_actionList.GetItemData(curAction);

    CString actionType, actionData;
    m_actionTypes.GetWindowText(actionType);
    TruncSpace(actionType);

    if (actionType.GetLength() == 0) {
        actionType = "0";
        m_actionTypes.SetWindowText(actionType);
    }

    bool actionChanged = false;
    CIniFile& ini = Map->GetIniFile();
    TriggerActions actions(ini.GetString("Actions", m_currentTrigger), triggerWpFilterFunc);
    auto& actionN = actions.Nth(curActionIdx);

    auto const newActionTypeIdx = atoi(actionType);
    if (newActionTypeIdx != actionN.actionType) {// only update if changes
        actionN.actionType = newActionTypeIdx;
        actionChanged = true;
    }

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& actionDef = triggerDefMgr.Actions().at(actionN.actionType);
    auto const& paramDefs = triggerDefMgr.Params();

    m_actionDescription.SetWindowText(actionDef.description);

    // handle control code:
    auto const newControlCode = std::abs(actionDef.controlCode);
    if (newControlCode != actionN.actionCode) {
        actionN.actionCode = newControlCode;
        actionChanged = true;
    }

    { // replace line in the list:
        m_actionList.DeleteString(curAction);
        auto const id = m_actionList.InsertString(
            curAction,
            makeEventShortDesc(curAction, actionDef.brief));
        m_actionList.SetItemData(id, curAction);
        m_actionList.SetCurSel(curAction);
    }

    // param setup
    for (auto i = 0; i < 5; i++) {
        if (auto paramType = actionDef.paramTypes[i]; paramType > 0) {
            auto const& paramDef = paramDefs.at(paramType);
            m_actionParamTexts[i]->SetWindowText(paramDef.paramName);
            HandleParamList(m_actionParam[i], paramDef.listType);
            m_actionParam[i].SetWindowText(actionN.params[i]);
            m_actionParam[i].EnableWindow(TRUE);
            continue;
        }
        CString translationLabel;
        translationLabel.Format("TriggerParameter#%dvalue", i + 1);
        m_actionParamTexts[i]->SetWindowText(TranslateStringACP(translationLabel));
        HandleParamList(m_actionParam[i], PARAMTYPE_NOTHING);
        m_actionParam[i].EnableWindow(FALSE);
    }

    auto const isWaypointFormat = IsWaypointFormat(actionN.waypoint);
    if (isWaypointFormat && !actionDef.useWaypoint) {
        actionN.waypoint.Format("%d", StringToWaypoint(actionN.waypoint));
        actionChanged = true;
    } else if (!isWaypointFormat) {
        actionN.waypoint = WaypointToString(atoi(actionN.waypoint));
        actionChanged = true;
    }

    // write back action at one time
    if (actionChanged) {
        ini.SetString("Actions", m_currentTrigger, actions.Serialize());
    }
}

std::pair<CString, CString> CTriggerEditorAllDlg::popUpCSFViewerAndReturn(CComboBox& cb)
{
    CString curValue;
    cb.GetWindowText(curValue);

    auto& csfDlg = theApp.MainWindow()->m_csfStrings;

    if (!curValue.IsEmpty() && curValue != "0") {
        TruncSpace(curValue);
        csfDlg.SetSelectedString(curValue);
    } else {
        csfDlg.SetSelectedString("");
    }

    CString label = csfDlg.DoModal() == IDCANCEL
        ? curValue : csfDlg.CSFLabelSelected();

    auto content = csfDlg.CSFContentSelected();
    auto const countCorrected = utf8ByteCount(content);

    return { label, content.Left(countCorrected) };
}

bool CTriggerEditorAllDlg::onEditChangeActionValueN(size_t nth, bool isFromDropDown)
{
    if (m_currentTrigger.GetLength() == 0) {
        return false;
    }
    int curAction = m_actionList.GetCurSel();
    if (curAction < 0) {
        return false;
    }
    int curActionIdx = m_actionList.GetItemData(curAction);
    bool actionChanged = false;
    CIniFile& ini = Map->GetIniFile();
    TriggerActions actions(ini.GetString("Actions", m_currentTrigger), triggerWpFilterFunc);
    auto& actionN = actions.Nth(curActionIdx);
    auto& actionParamCB = m_actionParam[nth];

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& actionDef = triggerDefMgr.Actions().at(actionN.actionType);
    auto const& paramDefN = triggerDefMgr.Params().at(actionDef.paramTypes[nth]);

    bool popUpHandled = false;
    CString newValue;
    if (paramDefN.listType == PARAMTYPE_TUTORIALTEXTS) {
        if (isFromDropDown) {
            auto [label, content] = popUpCSFViewerAndReturn(actionParamCB);
            auto txt = label;
            if (!content.IsEmpty()) {
                txt += ' ';
                txt += content;
            }
            newValue = label;
            popUpHandled = true;
        }
    } else {
        actionParamCB.GetWindowText(newValue);
        TruncSpace(newValue);
        if (actionDef.useWaypoint) {
            newValue = WaypointToString(atoi(newValue));
        }
    }

    if (newValue != actionN.params[nth]) {
        actionN.params[nth] = newValue;
        actionChanged = true;
    }

    if (actionChanged) {
        ini.SetString("Actions", m_currentTrigger, actions.Serialize());
    }
    return popUpHandled;
}

void CTriggerEditorAllDlg::onEditChangeActionValue1()
{
    onEditChangeActionValueN(0);
}

void CTriggerEditorAllDlg::onEditChangeActionValue2()
{
    onEditChangeActionValueN(1);
}

void CTriggerEditorAllDlg::onEditChangeActionValue3()
{
    onEditChangeActionValueN(2);
}

void CTriggerEditorAllDlg::onEditChangeActionValue4()
{
    onEditChangeActionValueN(3);
}

void CTriggerEditorAllDlg::onDropDownActionValue1()
{
    if (onEditChangeActionValueN(0, true)) {
        ::PostMessage(m_actionParam[0], CB_SHOWDROPDOWN, FALSE, 0);
    }
}

void CTriggerEditorAllDlg::onAddAction(TriggerAction&& action, int slot)
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    // TODO: verify
    CIniFile& ini = Map->GetIniFile();
    auto& sec = ini.AddSection("Actions");

    TriggerActions actions(sec.GetString(m_currentTrigger), triggerWpFilterFunc);
    actions.Insert(slot, std::move(action));

    sec.SetString(m_currentTrigger, actions.Serialize());

    UpdateDialog();

    m_actionList.SetCurSel(slot);
    onSelChangeAction();
}

void CTriggerEditorAllDlg::onNewAction()
{
    onAddAction(TriggerAction{
        .actionType = 0,
        .actionCode = 0,
        .params = {
            '0', '0', '0', '0', '0',
        }
    }, m_actionList.GetCount());
}

void CTriggerEditorAllDlg::onCloneAction()
{
}

void CTriggerEditorAllDlg::onDeleteAction()
{
}
