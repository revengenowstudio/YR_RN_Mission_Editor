#pragma once
#include <afxrich.h>

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

    void translateUI();
    void clear();
    void onOptionCheckChanged(const CButton& checkBtn, const int paramPos);

    CString m_currentTrigger;
    // trigger options
    // TODO: add a filter edit
    CComboBox m_triggerType;
    CEdit m_triggerName;
    CComboBox m_house;
    CComboBox m_persistence;
    CComboBox m_nextTrigger; // sub trigger, sharing my tag
    CButton	m_medium;
    CButton	m_hard;
    CButton	m_easy;
    CButton	m_disabled;
    // event options
    CComboBox m_eventTypes;
    CListBox m_eventList;
    CComboBox m_eventParam1;
    CComboBox m_eventParam2;
    CEdit m_eventDescription;
    // action options
    CComboBox m_actionTypes;
    CListBox m_actionList;
    CComboBox m_eventParam[6]; // 0-5
    CEdit m_actionDescription;
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedTrgrCloneTrigger();
};