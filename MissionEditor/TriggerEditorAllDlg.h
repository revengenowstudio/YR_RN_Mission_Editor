#pragma once
#include <afxrich.h>
#include "TriggerDef.h"

class CTriggerEditorAllDlg : public CDialog
{
public:
    enum { IDD = IDD_TRIGGER_ALL };
    static bool triggerWpFilterFunc(const CString& triggerCode);

    CTriggerEditorAllDlg(CWnd* pParent = nullptr);

    // used to update this dialog when other dialog has linked item changed
    void UpdateDialog();

protected:
    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;

    static std::pair<CString, CString> popUpCSFViewerAndReturn(CComboBox& cb);

    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void onNewTrigger();
    afx_msg void onDeleteTrigger();
    afx_msg void onPlaceOnMap();
    afx_msg void onSelChangeTrigger();
    afx_msg void onChangeTriggerName();
    afx_msg void onEditChangeHouse();
    afx_msg void onEditChangeNextTrigger();
    afx_msg void onKillFocusName();
    afx_msg void onKillFocus(CWnd* pNewWnd);
    afx_msg void onEditChangeTriggerType();
    afx_msg void OnDisabled();
    afx_msg void OnEasy();
    afx_msg void OnMedium();
    afx_msg void OnHard();

    afx_msg void onSelChangeOption(); // trigger selection changed, update option data only (not reset)
    afx_msg void onSelChangeEvent(); // eventType cb sel change
    afx_msg void onSelChangeAction();
    afx_msg void onEditChangeEventType(); // event list sel change
    afx_msg void onEditChangeEventValue(CMyComboBox& paramCB, size_t slot);
    afx_msg void onEditChangeEventValue1();
    afx_msg void onEditChangeEventValue2();
    afx_msg void onNewEvent();
    afx_msg void onCloneEvent();
    afx_msg void onDeleteEvent();

    afx_msg void onEditChangeActionType();
    bool onEditChangeActionValueN(size_t nth, bool isFromDropDown = false);
    afx_msg void onEditChangeActionValue1();
    afx_msg void onEditChangeActionValue2();
    afx_msg void onEditChangeActionValue3();
    afx_msg void onEditChangeActionValue4();
    afx_msg void onDropDownActionValue1();
    afx_msg void onNewAction();
    afx_msg void onCloneAction();
    afx_msg void onDeleteAction();

    afx_msg BOOL onMessageKeyDown(MSG* pMsg);

    void translateUI();
    void clear();
    void oneTimeInit();
    void onOptionCheckChanged(const CButton& checkBtn, const int paramPos);
    void parseTriggerDefinitions();
    void onAddEvent(TriggerEvent&& event, int slot);
    void onAddAction(TriggerAction&& action, int slot);

    void updateTriggerOptions();
    void updateTriggerEvents();
    void updateTriggerActions();



    CString m_currentTrigger;
    // trigger options
    // TODO: add a filter edit
    CComboBox m_triggerType;
    CEdit m_triggerName;
    CMyComboBox m_house;
    CMyComboBox m_persistence;
    CComboBox m_nextTrigger; // sub trigger, sharing my tag
    CButton	m_medium;
    CButton	m_hard;
    CButton	m_easy;
    CButton	m_disabled;
    // event options
    CMyComboBox m_eventTypes;
    CListBox m_eventList;
    CMyComboBox m_eventParam1;
    CMyComboBox m_eventParam2;
    CEdit m_eventDescription;
    // action options
    CMyComboBox m_actionTypes;
    CListBox m_actionList;
    CWnd* m_actionParamTexts[6];
    CMyComboBox m_actionParam[6]; // 0-5
    CEdit m_actionDescription;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void onCloneTrigger();
};