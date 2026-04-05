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
    ON_BN_CLICKED(IDC_TRGR_MUST_TRANSFER, OnBnClickedTrgrMustTransfer)
    ON_BN_CLICKED(IDC_TRGR_DISABLED, OnDisabled)
    ON_BN_CLICKED(IDC_TRGR_EASY, OnEasy)
    ON_BN_CLICKED(IDC_TRGR_MEDIUM, OnMedium)
    ON_BN_CLICKED(IDC_TRGR_HARD, OnHard)
    ON_EN_KILLFOCUS(IDC_TRGR_NAME, onChangeTriggerName)
    ON_CBN_EDITCHANGE(IDC_TRGR_HOUSE, onEditChangeHouse)
    ON_CBN_EDITCHANGE(IDC_TRGR_TYPE, onChangePersistence)
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
    ON_CBN_EDITCHANGE(IDC_TRGR_ATTACHED_TRIGGER, onEditAttachedTrigger)
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

    oneTimeInit();

    return TRUE;
}

void CTriggerEditorAllDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_TRGR_SELECTED_TRIGGER, m_triggerType);
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
    TranslateDlgItem(*this, IDC_TRGR_MUST_TRANSFER, "TriggerOptionMustTransfer");
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

void CTriggerEditorAllDlg::clearTriggerTypes()
{
    while (m_triggerType.DeleteString(0) != CB_ERR);
    m_currentTrigger.Reset();
}

void CTriggerEditorAllDlg::clear()
{
    clearTriggerTypes();
    while (m_eventList.DeleteString(0) != CB_ERR);
    while (m_actionList.DeleteString(0) != CB_ERR);
    while (m_house.DeleteString(0) != CB_ERR);
    while (m_nextTrigger.DeleteString(0) != CB_ERR);
    m_eventTypes.SetCurSel(CB_ERR);
    m_actionTypes.SetCurSel(CB_ERR);
    m_eventTypes.SetWindowText("");
    m_actionTypes.SetWindowText("");
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

// The first reference value is designed to save memory during loop
static void makeTriggerDisplay(CString& ret,  const CString& name, const CString& id)
{
    ret.Format("%s (%s)", name, id);
}

void listTriggers(CComboBox& cb)
{
    while (cb.DeleteString(0) != CB_ERR);

    auto const& triggersSec = DB::Triggers.CustomIndex();

    CString triggerDisplay;
    auto idx = 0;
    for (auto it = triggersSec.begin(); it != triggersSec.end(); it++, idx++) {
        auto const& [name, id] = *it;
        makeTriggerDisplay(triggerDisplay, name, id);
        int pos = cb.AddString(triggerDisplay);
        cb.SetItemData(pos, idx);
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
    if (m_currentTrigger.Get().IsEmpty()) {
        while (m_eventList.DeleteString(0) != CB_ERR);
        return;
    }

    int cur_sel = m_eventList.GetCurSel();
    while (m_eventList.DeleteString(0) != CB_ERR);

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();
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
    if (m_currentTrigger.Get().IsEmpty()) {
        while (m_actionList.DeleteString(0) != CB_ERR);
        return;
    }

    int cur_sel = m_actionList.GetCurSel();
    while (m_actionList.DeleteString(0) != CB_ERR);

    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
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

void CTriggerEditorAllDlg::resetTriggerTypeList()
{
    clearTriggerTypes();
    if (m_triggerType.GetCount() <= 0) {

        listTriggers(m_triggerType);

        if (m_triggerType.GetCount() > 0) {
            m_triggerType.SetCurSel(0);
            CString displayStr;
            m_triggerType.GetWindowText(displayStr);
            m_currentTrigger.Set(displayStr);
        }
    }
}

void CTriggerEditorAllDlg::UpdateDialog()
{
    clear();
    // means first time open, try load trigger types
    resetTriggerTypeList();

    updateTriggerOptions();
    updateTriggerEvents();
    updateTriggerActions();
}

void CTriggerEditorAllDlg::onAddTrigger(TriggerInstance&& trigger)
{
    auto const id = trigger.ID();
    auto const name = trigger.Options().Name();
    DB::Triggers.Append(std::move(trigger));

    auto& tag = DB::Tags.Append(GetFreeID(), name + " Tag");
    tag.triggerId = id;

    clear();
    // means first time open, try load trigger types
    resetTriggerTypeList();
    updateTriggerOptions();

    CString displayStr;
    makeTriggerDisplay(displayStr, name, id);

    auto const selected = m_triggerType.FindStringExact(0, displayStr);
    if (selected == CB_ERR) {
        errstream << "Trigger " << id << " renamed to " << name << " but could not be found anymore";
        ASSERT(FALSE);
        return;
    }
    m_triggerType.SetCurSel(selected);
    onSelChangeTrigger();
}

void CTriggerEditorAllDlg::onNewTrigger()
{
    TriggerInstance inst(GetFreeID(), "New trigger", Map->GetHouseID(0, TRUE));
    onAddTrigger(std::move(inst));
}

void CTriggerEditorAllDlg::onCloneTrigger()
{
    int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }

    auto trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    // NOTE: index should not update now since 
    trigger.SetName(trigger.Options().Name() + " Clone", false);
    trigger.SetID(GetFreeID());
    onAddTrigger(std::move(trigger));
}

void CTriggerEditorAllDlg::onDeleteTrigger()
{
    const int sel = m_triggerType.GetCurSel();
    if (sel < 0) {
        return;
    }
    auto const title = TranslateStringACP("Delete trigger");
    auto const content = EscapeString(TranslateStringACP("TriggerDeleteTip"));
    int res = MessageBox(content, title, MB_YESNOCANCEL);
    if (res == IDCANCEL) {
        return;
    }

    auto const triggerId = m_currentTrigger.Get();

    // YES means clean tags, otherwise ignore
    if (res == IDYES) {
        std::vector<CString> keysToDelete;
        auto& tagDb = DB::Tags;
        for (auto const& tag : tagDb) {
            if (triggerId == tag.triggerId) {
                keysToDelete.push_back(tag.id);
            }
        }
        for (auto const& keyToDelete : keysToDelete) {
            tagDb.DeleteByID(keyToDelete);
        }
    }

    auto const deleted = DB::Triggers.DeleteByID(triggerId);
    ASSERT(deleted);

    resetTriggerTypeList();

    int nextSel = sel - 1; // 0 will be -1, means no selection
    if (nextSel < 0 && m_triggerType.GetCount() > 0) { // still having item left, choose first
        nextSel = 0;
    }
    m_triggerType.SetCurSel(nextSel);

    onSelChangeTrigger();
}

void CTriggerEditorAllDlg::onPlaceOnMap()
{
    auto const triggerId = m_currentTrigger.Get();
    if (triggerId.IsEmpty()) {
        return;
    }

    CString tag;

    auto const& tagDb = DB::Tags;
    for (auto const& tagN : tagDb) {
        if (triggerId == tagN.triggerId) {
            tag = tagN.id;
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
    CString displayStr;
    m_triggerType.GetLBText(curSel, displayStr);
    m_currentTrigger.Set(displayStr);
    //errstream << "onSelChangeTrigger - displayStr: " << displayStr;

    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    onSelChangeOption();
    updateTriggerEvents();
    updateTriggerActions(); // update, would you?
}

void CTriggerEditorAllDlg::onChangeTriggerName()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    CString newName;
    GetDlgItem(IDC_TRGR_NAME)->GetWindowText(newName);

    if (newName.IsEmpty()) {
        newName = " ";
    }

    if (newName.Find(',', 0) >= 0) {
        newName.Trim(',');
        GetDlgItem(IDC_TRGR_NAME)->SetWindowText(newName);
    }

    auto& trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    trigger.SetName(newName, true);

    int i;
    int p = 0;
    // update tag name
    auto& tagDb = DB::Tags;
    for (auto& tag: tagDb) {
        if (tag.triggerId == m_currentTrigger.Get()) {
            tag.name = newName; 
            tag.name += " 1";
            break; // will there be multiple tags point to the same trigger?
        }
    }

    onKillFocusName();
}

void CTriggerEditorAllDlg::onChangePersistence()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    CString persistenceStr;
    m_persistence.GetWindowText(persistenceStr);
    persistenceStr.Trim();

    auto& tagDb = DB::Tags;
    // locate that tag and update its value
    for (auto& tag : tagDb) {
        if (tag.triggerId == m_currentTrigger.Get()) {
            tag.persistence = atoi(persistenceStr);
            break;
        }
    }

}

void CTriggerEditorAllDlg::onEditChangeHouse()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    CString newHouse;
    m_house.GetWindowText(newHouse);

    newHouse = TranslateHouse(newHouse);

    newHouse.TrimLeft();
    TruncSpace(newHouse);
    newHouse.Trim(',');
    auto& trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    trigger.Options().house = newHouse;
}

void CTriggerEditorAllDlg::onEditChangeNextTrigger()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    CString newTrigger;
    m_nextTrigger.GetWindowText(newTrigger);
    newTrigger.TrimLeft();
    TruncSpace(newTrigger);

    if (newTrigger.Find(",", 0) >= 0) {
        newTrigger.SetAt(newTrigger.Find(",", 0), 0);
    }
    // TODO: validate newTrigger, to avoid loop
    auto& trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    trigger.Options().nextTrigger = newTrigger;
}

void CTriggerEditorAllDlg::onKillFocusName()
{
    // cache trigger type index
    auto const id = m_currentTrigger.Get();

    UpdateDialog();

    auto const& name = DB::Triggers.Lookup(id).Options().Name();

    CString displayStr;
    makeTriggerDisplay(displayStr, name, id);


    auto const selected = m_triggerType.FindStringExact(0, displayStr);
    if (selected == CB_ERR) {
        errstream << "Trigger " << id << " renamed to " << name << " but could not be found anymore";
        ASSERT(FALSE);
        return;
    }

    m_triggerType.SetCurSel(selected);
    onSelChangeTrigger();
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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }
    auto const checked = checkBtn.GetCheck() == BST_CHECKED;
    auto& trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    trigger.Options().controls[control] = checked;
}

void CTriggerEditorAllDlg::ParseTriggerDefinitions()
{
    TriggerDefinitionManager::Instance().LoadFrom(g_data, errstream);
}

void CTriggerEditorAllDlg::OnBnClickedTrgrMustTransfer()
{
    // not using DDX binding because it causing Chinese character missing
    auto& btn = *reinterpret_cast<CButton*>(GetDlgItem(IDC_TRGR_MUST_TRANSFER));
    onOptionCheckChanged(btn, TriggerOptions::MustTransfer);
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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }
    auto& trigger = DB::Triggers.Lookup(m_currentTrigger.Get());
    auto& options = trigger.Options();

    GetDlgItem(IDC_TRGR_NAME)->SetWindowText(options.Name());
    m_house.SetWindowText(TranslateHouse(options.house, TRUE));
    CString attachedTrigger = options.nextTrigger;
    m_nextTrigger.SetWindowText(attachedTrigger);

    reinterpret_cast<CButton*>(GetDlgItem(IDC_TRGR_MUST_TRANSFER))->SetCheck(options.controls[TriggerOptions::MustTransfer]);
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

    auto const& tagDb = DB::Tags;
    for (auto const& tag: tagDb) {
        if (tag.triggerId == m_currentTrigger.Get()) {
            // update persistence
            m_persistence.SetWindowText(tag.PersistenceString());
            break;
        }
    }
}

// ========================== Trigger Events ==========================
void CTriggerEditorAllDlg::onSelChangeEvent()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();

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
    if (m_currentTrigger.Get().IsEmpty()) {
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
    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();
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
    GetDlgItem(IDC_TRGR_EVENT_P1_TXT)->SetWindowText(paramType1.paramName);
    GetDlgItem(IDC_TRGR_EVENT_P2_TXT)->SetWindowText(paramType2.paramName);

    m_eventParam1.SetWindowText(eventData.param1);

    if (eventData.param2.has_value()) {
        m_eventParam2.SetWindowText(eventData.param2.value());
    }

    m_eventParam1.EnableWindow(eventDef.paramTypes[0] > 0);
    m_eventParam2.EnableWindow(eventDef.paramTypes[1] > 0);
}

void CTriggerEditorAllDlg::onEditChangeEventValue(CMyComboBox& paramCB, size_t slot)
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();
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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();

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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();

    onAddEvent(TriggerEvent(events.Nth(eventIdx)), eventIdx + 1);
}

void CTriggerEditorAllDlg::onDeleteEvent()
{
    auto const title = TranslateStringACP("Delete event");
    auto const content = TranslateStringACP("Do you really want to delete this event?");
    if (MessageBox(content, title, MB_YESNO) == IDNO) {
        return;
    }

    if (m_currentTrigger.Get().GetLength() == 0) {
        return;
    }

    int eventIdx = m_eventList.GetCurSel();
    if (eventIdx < 0) {
        return;
    }

    TriggerEvents& events = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Events();
    events.DeleteAt(eventIdx);

    updateTriggerEvents();
    if (m_eventList.GetCount() > 0) {
        m_eventList.SetCurSel(std::max(eventIdx - 1, 0));
    }
}

// ========================== Trigger Actions ==========================
void CTriggerEditorAllDlg::onSelChangeAction()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
    auto const& actionN = actions.Nth(actionIdx);

    //m_actionTypes.SetWindowText(makeEventShortDesc(actionN.actionType, actionDef.brief));

    for (auto idx = 0; idx < m_actionTypes.GetCount(); idx++) {
        if (m_actionTypes.GetItemData(idx) == actionN.ActionType()) {
            m_actionTypes.SetCurSel(idx);
        }
    }

    onEditChangeActionType();
}

void handleParamList(CComboBox& cb, const ParamType& paramType)
{
    // legacy way
    if (paramType.sequencedValues.empty()) {
        HandleParamList(cb, paramType.listType);
        return;
    }
    // customized values
    while (cb.DeleteString(0) != CB_ERR);
    for (auto const& item : paramType.sequencedValues) {
        cb.AddString(item);
    }
}

void CTriggerEditorAllDlg::onEditChangeActionType()
{
    if (m_currentTrigger.Get().GetLength() == 0) {
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
    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
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
        handleParamList(m_actionParam[slot], paramDef);
        m_actionParam[slot].SetWindowText(actionN.Params()[paramSlot]); // NOTE: this slot can start from 1, not always 0
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
    if (m_currentTrigger.Get().GetLength() == 0) {
        return false;
    }
    int curActionIdx = m_actionList.GetCurSel();
    if (curActionIdx < 0) {
        return false;
    }
    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }

    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
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
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();

    onAddAction(TriggerAction(actions.Nth(actionIdx)), actionIdx + 1);
}

void CTriggerEditorAllDlg::onDeleteAction()
{
    auto const title = TranslateStringACP("Delete action");
    auto const content = TranslateStringACP("Do you really want to delete this action?");
    if (MessageBox(content, title, MB_YESNO) == IDNO) {
        return;
    }
    if (m_currentTrigger.Get().GetLength() == 0) {
        return;
    }
    int actionIdx = m_actionList.GetCurSel();
    if (actionIdx < 0) {
        return;
    }
    TriggerActions& actions = DB::Triggers.
        Lookup(m_currentTrigger.Get()).Actions();
    actions.DeleteAt(actionIdx);

    updateTriggerActions();
    if (m_actionList.GetCount() > 0) {
        m_actionList.SetCurSel(std::max(actionIdx - 1, 0));
    }
}

void CTriggerEditorAllDlg::onEditAttachedTrigger()
{
    if (m_currentTrigger.Get().IsEmpty()) {
        return;
    }
    auto const curSel = m_nextTrigger.GetCurSel();
    if (curSel < 0) {
        return;
    }

    CString triggerId;
    m_nextTrigger.GetWindowText(triggerId);
    TruncSpace(triggerId);

    auto& db = DB::Triggers;
    if (triggerId != "<none>" && !db.Exists(triggerId)) {
        MessageBox(TranslateStringACP("TriggerOptionInvalidID"), TranslateStringACP("Error"));
        return;
    }
    db.Lookup(m_currentTrigger.Get()).Options().nextTrigger = triggerId;
}
