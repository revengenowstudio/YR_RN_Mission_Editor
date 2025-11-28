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

    void translateUI();
    void clear();

    // trigger options
    // TODO: add a filter edit
    CComboBox m_selectedTrigger;
    CEdit m_triggerName;
    CComboBox m_house;
    CComboBox m_persistence;
    CComboBox m_nextTrigger; // sub trigger, sharing my tag
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
};