#include "StdAfx.h"
#include "TriggerEditorAllDlg.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"
#include "TriggerDatabase.h"

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
    TranslateDlgItem(*this, IDC_TRGR_ACTION_OPTIONS, "TriggerActionOptions");
    TranslateDlgItem(*this, IDC_TRGR_ACTION_TYPE_TXT, "TriggerActionType");
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
    m_currentTrigger.Empty();

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
        auto const id = m_eventTypes.AddString(eventdata.brief);
        m_eventTypes.SetItemData(id, eventid);
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
        auto const id = m_actionTypes.AddString(actionDef.brief);
        m_actionTypes.SetItemData(id, actionId);
    }
}

void listTriggers(CComboBox& cb)
{
    CIniFile& ini = Map->GetIniFile();

    while (cb.DeleteString(0) != CB_ERR);
    auto const& triggersSec = TriggerDatabase::Instance();

    CString triggerDisplay;
    for (auto idx = 0; idx < triggersSec.Size(); idx++) {
        auto const& inst = triggersSec.Nth(idx);
        triggerDisplay.Format("%s (%s)", inst.ID(), inst.Options().name);
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

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();
    auto const eventCount = events.Size();

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto i = 0; i < eventCount; i++) {
        auto const eventIdx = events.Nth(i).eventType;
        auto const& brief = defMgr.Events().at(eventIdx).brief;
        m_eventList.AddString(makeEventShortDesc(i, brief));
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

    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    auto const actionCount = actions.Size();

    auto& defMgr = TriggerDefinitionManager::Instance();
    for (auto i = 0; i < actionCount; i++) {
        auto const& brief = actions.Nth(i).Type().brief;
        m_actionList.AddString(makeEventShortDesc(i, brief));
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

void CTriggerEditorAllDlg::onAddTrigger(TriggerInstance&& trigger)
{
    auto const id = trigger.ID();
    auto const name = trigger.Options().name;
    TriggerDatabase::Instance().
        Append(std::move(trigger));

    // add tag, TODO: make Tag as object
    {
        CIniFile& ini = Map->GetIniFile();
        CString tagId = GetFreeID();
        CString content;
        content.Format("0,%s tag,%s", name, id);
        ini.SetString("Tags", tagId, content);
    }

    theApp.MainWindow()->UpdateDialogs(TRUE);

    for (auto i = 0; i < m_triggerType.GetCount(); i++) {
        if (m_triggerType.GetItemData(i) == TriggerDatabase::Instance().FindIndex(id)) {
            m_triggerType.SetCurSel(i);
        }
    }
    onSelChangeTrigger();
}

void CTriggerEditorAllDlg::onNewTrigger()
{
    TriggerInstance inst(GetFreeID(), "New trigger", Map->GetHouseID(0, TRUE));
    onAddTrigger(std::move(inst));
}

void CTriggerEditorAllDlg::onCloneTrigger()
{
    CIniFile& ini = Map->GetIniFile();

    int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }
    int triggerIdx = m_triggerType.GetItemData(sel);

    auto trigger = TriggerDatabase::Instance().Nth(triggerIdx);
    trigger.Options().name += " Clone";
    trigger.SetID(GetFreeID());
    onAddTrigger(std::move(trigger));
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

    auto const triggerId = TriggerDatabase::Instance().Nth(curTrigger).ID();

    CIniFile& ini = Map->GetIniFile();
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

    TriggerDatabase::Instance().DeleteAt(curTrigger);
    if (TriggerDatabase::Instance().Size() == 0) {
        m_triggerType.SetWindowText("");
        m_triggerType.SetCurSel(-1);
    }

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
    auto const triggerId = TriggerDatabase::Instance().Nth(curtrig).ID();
    CString tag;

    for (auto const& [type, def] : ini["Tags"]) {
        CString attTrigg = GetParam(def, 2);
        if (triggerId == attTrigg) {
            tag = type;
            break;
        }
    }

    if (tag.IsEmpty()) {
        return;
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
    m_currentTrigger = TriggerDatabase::Instance().Nth(curInd).ID();

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
    onSelChangeOption();
    updateTriggerEvents();
    updateTriggerActions(); // update, would you?
}

void CTriggerEditorAllDlg::onChangeTriggerName()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CString newName;
    m_triggerName.GetWindowText(newName);

    if (newName.IsEmpty()) {
        newName = " ";
    }

    if (newName.Find(',', 0) >= 0) {
        newName.Trim(',');
        m_triggerName.SetWindowText(newName);
    }

    auto& trigger = TriggerDatabase::Instance().Lookup(m_currentTrigger);
    trigger.Options().name = newName;

    int i;
    int p = 0;
    auto& ini = Map->GetIniFile();
    // update tag name
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

    onKillFocusName();
}

void CTriggerEditorAllDlg::onEditChangeHouse()
{
    CIniFile& ini = Map->GetIniFile();

    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CString newHouse;
    m_house.GetWindowText(newHouse);

    newHouse = TranslateHouse(newHouse);

    newHouse.TrimLeft();
    TruncSpace(newHouse);
    newHouse.Trim(',');
    auto& trigger = TriggerDatabase::Instance().Lookup(m_currentTrigger);
    trigger.Options().house = newHouse;
}

void CTriggerEditorAllDlg::onEditChangeNextTrigger()
{
    CIniFile& ini = Map->GetIniFile();

    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    CString newTrigger;
    m_nextTrigger.GetWindowText(newTrigger);
    newTrigger.TrimLeft();
    TruncSpace(newTrigger);

    if (newTrigger.Find(",", 0) >= 0) {
        newTrigger.SetAt(newTrigger.Find(",", 0), 0);
    }

    auto& trigger = TriggerDatabase::Instance().Lookup(m_currentTrigger);
    trigger.Options().nextTrigger = newTrigger;
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

void CTriggerEditorAllDlg::onOptionCheckChanged(
    const CButton& checkBtn,
    const TriggerOptions::Controls control
)
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    auto const checked = checkBtn.GetCheck() != 0;
    auto& trigger = TriggerDatabase::Instance().Lookup(m_currentTrigger);
    trigger.Options().controls[control] = checked;
}

void CTriggerEditorAllDlg::parseTriggerDefinitions()
{
    TriggerDefinitionManager::Instance().LoadFrom(g_data, errstream);
}

void CTriggerEditorAllDlg::OnDisabled()
{
    onOptionCheckChanged(m_disabled, TriggerOptions::Disable);
}

void CTriggerEditorAllDlg::OnEasy()
{
    onOptionCheckChanged(m_easy, TriggerOptions::Easy);
}

void CTriggerEditorAllDlg::OnMedium()
{
    onOptionCheckChanged(m_medium, TriggerOptions::Medium);
}

void CTriggerEditorAllDlg::OnHard()
{
    onOptionCheckChanged(m_hard, TriggerOptions::Hard);
}

void CTriggerEditorAllDlg::onSelChangeOption()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    auto& trigger = TriggerDatabase::Instance().Lookup(m_currentTrigger);
    auto& options = trigger.Options();

    m_triggerName.SetWindowText(options.name);
    m_house.SetWindowText(TranslateHouse(options.house, TRUE));
    CString attachedTrigger = options.nextTrigger;
    m_nextTrigger.SetWindowText(attachedTrigger);

    m_disabled.SetCheck(options.controls[TriggerOptions::Disable]);
    m_easy.SetCheck(options.controls[TriggerOptions::Easy]);
    m_medium.SetCheck(options.controls[TriggerOptions::Medium]);
    m_hard.SetCheck(options.controls[TriggerOptions::Hard]);

    for (auto i = 0; i < m_nextTrigger.GetCount(); i++) {
        CString tmp;
        m_nextTrigger.GetLBText(i, tmp);
        TruncSpace(tmp);
        if (tmp == attachedTrigger) {
            m_nextTrigger.SetCurSel(i);
        }
    }

    auto const& ini = Map->GetIniFile();
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
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();

    auto const& eventData = events.Nth(eventIdx);

    CString tmp;
    for (auto i = 0; i < m_eventTypes.GetCount(); i++) {
        if (m_eventTypes.GetItemData(i) == eventData.eventType) {
            m_eventTypes.SetCurSel(i);
        }
    }

    onEditChangeEventType();
}

// TODO: support filter, split out edit change and select change
void CTriggerEditorAllDlg::onEditChangeEventType()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }
    CString eventtype;
    m_eventTypes.GetWindowText(eventtype);
    TruncSpace(eventtype);

    // not sure necessary, maybe for new event
    if (eventtype.IsEmpty()) {
        eventtype = "0";
        m_eventTypes.SetWindowText(eventtype);
    }

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();
    auto& eventData = events.Nth(eventIdx);

    // validate eventType to be int
    // this validation will be changed once filter applied
    auto const eventTypeIdx = atoi(eventtype);
    if (eventTypeIdx < 0
        || !triggerDefMgr.Events().contains(eventTypeIdx)
        || !IsNumeric(eventtype)) {
        eventtype.Format("%d", eventData.eventType);
        m_eventTypes.SetWindowText(eventtype); // set back old value
        MessageBox(TranslateStringACP("TriggerEventInvalidType"), TranslateStringACP("Error"));
        return;
    }

    bool is4SlotEvent = false; // keep it for now, may not be necessary any more, we have safer serde
    if (eventData.param2.has_value()) {
        is4SlotEvent = true;
    }
    eventData.eventType = eventTypeIdx; // apply new event type Idx

    auto const& eventDef = triggerDefMgr.Events().at(eventData.eventType);
    auto const& paramType1 = triggerDefMgr.Params().at(eventDef.paramTypes[0]);
    auto const& paramType2 = triggerDefMgr.Params().at(eventDef.paramTypes[1]);

    { // replace line in the list:
        m_eventList.DeleteString(eventIdx);
        m_eventList.InsertString(
            eventIdx,
            makeEventShortDesc(eventIdx, eventDef.brief));
        m_eventList.SetCurSel(eventIdx);
    }

    if (paramType2.slotCount < 2) {// clear this slot if switching to 1 slot param
        eventData.param2.reset();
    } else if (!eventData.param2.has_value()) {// if there was no such value, give it a '0' value
        eventData.param2.emplace("0");
    }
    // otherwise, keep its value, no change

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

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();
    auto& eventData = events.Nth(eventIdx);

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& eventDef = triggerDefMgr.Events().at(eventData.eventType);
    auto const& paramType = triggerDefMgr.Params().at(eventDef.paramTypes[slot]);

    CString newVal;
    paramCB.GetWindowText(newVal);
    TruncSpace(newVal);
    newVal.Trim();
    newVal.Trim(',');

    // validate if item matches anything in
#if 0 // disabled because incomplete typing would cause this too
    auto const itemCount = paramCB.GetCount();
    CString itemData;
    bool newValValid = itemCount == 0;
    for (auto idx = 0; idx < itemCount; ++idx) {
        paramCB.GetLBText(idx, itemData);
        TruncSpace(itemData);
        if (newVal == itemData) {
            newValValid = true;
            break;
        }
    }

    if (!newValValid) {
        auto const choice = MessageBox(TranslateStringACP("TriggerEventParamNotInSet"),
            TranslateStringACP("Error"), MB_YESNO);
        if (choice != IDYES) {
            newVal = slot == 0 ?
                eventData.param1 : eventData.param2.value_or("");
            paramCB.SetWindowText(newVal);
            return;
        }
    }
#endif

    slot == 0 ? eventData.param1 = newVal
        : eventData.param2 = newVal;
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

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();

    events.Insert(slot, std::move(event));

    //m_eventList.InsertString(
    //    eventIdx,
    //    makeEventShortDesc(eventIdx, eventDef.brief));
    // use for loop to update following events after this insert index

    updateTriggerEvents(); // TODO: optimize, only update eventList

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

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();

    onAddEvent(TriggerEvent(events.Nth(eventIdx)), eventIdx + 1);
}

void CTriggerEditorAllDlg::onDeleteEvent()
{
    auto const title = TranslateStringACP("Delete event");
    auto const content = TranslateStringACP("Do you really want to delete this event?");
    if (MessageBox(content, title, MB_YESNO) == IDNO) {
        return;
    }

    if (m_currentTrigger.GetLength() == 0) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Events();
    events.DeleteAt(eventIdx);

    updateTriggerEvents();
    if (m_eventList.GetCount() > 0) {
        m_eventList.SetCurSel(eventIdx - 1);
    }
}

// ========================== Trigger Actions ==========================
void CTriggerEditorAllDlg::onSelChangeAction()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    auto const& actionN = actions.Nth(actionIdx);

    //m_actionTypes.SetWindowText(makeEventShortDesc(actionN.actionType, actionDef.brief));

    for (auto idx = 0; idx < m_actionTypes.GetCount(); idx++) {
        if (m_actionTypes.GetItemData(idx) == actionN.ActionType()) {
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
    int curActionIdx = m_actionList.GetCurSel();
    if (curActionIdx < 0) {
        return;
    }
    CString actionType;
    m_actionTypes.GetWindowText(actionType);
    TruncSpace(actionType);

    if (actionType.GetLength() == 0) {
        actionType = "0";
        m_actionTypes.SetWindowText(actionType);
    }

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    auto& actionN = actions.Nth(curActionIdx);

    // validate eventType to be int
// this validation will be changed once filter applied
    auto const actionTypeIdx = atoi(actionType);
    if (actionTypeIdx < 0
        || !triggerDefMgr.Actions().contains(actionTypeIdx)
        || !IsNumeric(actionType)) {
        actionType.Format("%d", actionN.ActionType());
        m_actionTypes.SetWindowText(actionType); // set back old value
        MessageBox(TranslateStringACP("TriggerActionInvalidType"), TranslateStringACP("Error"));
        return;
    }

    auto const newActionTypeIdx = actionTypeIdx;
    actionN.SetActionType(newActionTypeIdx);// only update if changes

    auto const& actionDef = actionN.Type();
    auto const& paramDefs = triggerDefMgr.Params();

    m_actionDescription.SetWindowText(actionDef.description);

    { // replace line in the list:
        m_actionList.DeleteString(curActionIdx);
        auto const id = m_actionList.InsertString(
            curActionIdx,
            makeEventShortDesc(curActionIdx, actionDef.brief));
        m_actionList.SetCurSel(curActionIdx);
    }

    bool validSlots[5] = { false };

    int slot = 0;
    for (auto const& [paramSlot, paramType] : actionDef.paramTypes) {
        auto const& paramDef = paramDefs.at(paramType);
        m_actionParamTexts[slot]->SetWindowText(paramDef.paramName);
        HandleParamList(m_actionParam[slot], paramDef.listType);
        m_actionParam[slot].SetWindowText(actionN.Params()[paramSlot]);
        m_actionParam[slot].EnableWindow(TRUE);
        validSlots[slot] = true;
        slot++;
    }

    // continue use last slot
    if (actionDef.useWaypointSlot) {
        if (actionN.IsUsingWaypointEncoding()) {
            m_actionParamTexts[slot]->SetWindowText(TranslateStringACP("Waypoint"));
            HandleParamList(m_actionParam[slot], PARAMTYPE_WAYPOINTS);
        } else {
            m_actionParamTexts[slot]->SetWindowText(TranslateStringACP("Number"));
            HandleParamList(m_actionParam[slot], PARAMTYPE_NOTHING);
        }
        CString numStr;
        numStr.Format("%d", actionN.Waypoint());
        m_actionParam[slot].SetWindowText(numStr);
        m_actionParam[slot].EnableWindow(TRUE);
        validSlots[slot] = true;
        slot++;
    }

    // reset unused
    for (auto i = 0; i < 5; i++) {
        if (validSlots[i]) {
            continue;
        }
        CString translationLabel;
        translationLabel.Format("TriggerParameter#%dvalue", i + 1);
        m_actionParamTexts[i]->SetWindowText(TranslateStringACP(translationLabel));
        HandleParamList(m_actionParam[i], PARAMTYPE_NOTHING);
        m_actionParam[i].SetWindowText("");
        m_actionParam[i].EnableWindow(FALSE);
    }

    // seems no action uses tag
    if (actionDef.useTag) {

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
    int curActionIdx = m_actionList.GetCurSel();
    if (curActionIdx < 0) {
        return false;
    }
    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    auto& actionN = actions.Nth(curActionIdx);
    auto& actionParamCB = m_actionParam[nth];

    auto const& triggerDefMgr = TriggerDefinitionManager::Instance();
    auto const& actionDef = actionN.Type();
    auto const listType = actionN.ParamNth(nth).ListType();

    bool popUpHandled = false;
    CString newValue;
    if (listType == PARAMTYPE_TUTORIALTEXTS && isFromDropDown) {
        auto const [label, content] = popUpCSFViewerAndReturn(actionParamCB);
        auto txt = label;
        if (!content.IsEmpty()) {
            txt += ' ';
            txt += content;
        }
        newValue = label;
        popUpHandled = true;
        actionParamCB.SetWindowText(label);
    } else {
        actionParamCB.GetWindowText(newValue);
        TruncSpace(newValue);
    }

    auto const lenBeforeTrim = newValue.GetLength();
    newValue.Trim(',');
    if (newValue.GetLength() != lenBeforeTrim) {
        actionParamCB.SetWindowText(newValue);
    }

    if (listType == PARAMTYPE_WAYPOINTS && actionDef.useWaypointSlot) {
        actionN.SetWaypoint(atoi(newValue));
    }
    else {
        actionN.ParamNth(nth).Assign(newValue);
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

    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    actions.Insert(slot, std::move(action));

    updateTriggerActions(); // TODO: optimize, only update actionList

    m_actionList.SetCurSel(slot);
    onSelChangeAction();
}

void CTriggerEditorAllDlg::onNewAction()
{
    onAddAction(TriggerAction(), m_actionList.GetCount());
}

void CTriggerEditorAllDlg::onCloneAction()
{
    if (m_currentTrigger.IsEmpty()) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();

    onAddAction(TriggerAction(actions.Nth(actionIdx)), actionIdx + 1);
}

void CTriggerEditorAllDlg::onDeleteAction()
{
    auto const title = TranslateStringACP("Delete action");
    auto const content = TranslateStringACP("Do you really want to delete this action?");
    if (MessageBox(content, title, MB_YESNO) == IDNO) {
        return;
    }
    if (m_currentTrigger.GetLength() == 0) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = TriggerDatabase::Instance().
        Lookup(m_currentTrigger).Actions();
    actions.DeleteAt(actionIdx);

    updateTriggerActions();
    if (m_actionList.GetCount() > 0) {
        m_actionList.SetCurSel(actionIdx - 1);
    }
}
