#pragma once
#include <afxrich.h>
#include "TriggerDef.h"

class CTriggerEditorAllDlg : public CDialog
{
public:
    enum { IDD = IDD_TRIGGER_ALL };

    CTriggerEditorAllDlg(CWnd* pParent = nullptr);

    // used to update this dialog when other dialog has linked item changed
    void UpdateDialog();

protected:
    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;

    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void onNewTrigger();
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

    void translateUI();
    void clear();
    void oneTimeInit();
    void onOptionCheckChanged(const CButton& checkBtn, const int paramPos);
    void parseTriggerDefinitions();

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
    CMyComboBox m_actionParam[6]; // 0-5
    CEdit m_actionDescription;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedTrgrCloneTrigger();
};