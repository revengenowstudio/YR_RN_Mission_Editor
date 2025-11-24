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

    void translateUI();

};