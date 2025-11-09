#pragma once

#include <map>
#include <vector>
#include <string>
#include <regex>
#include <functional>
#include <afxrich.h>
#include "structs.h"

// A static window class
class CCsfViewer : public CDialog
{
public:
    static LRESULT CALLBACK ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    enum { IDD = IDD_CSF_VIEWER };

    CCsfViewer(CWnd* pParent = nullptr);
    ~CCsfViewer();

    CString CSFLabelSelected() const
    {
        return CurrentSelectedCSF;
    }
    void SetSelectedString(CString str);

protected:
    enum Controls {
        SelectedCSF = 1001,
        NewFile = 1002,
        Search = IDC_CSF_VIEW_SELECTED,
        //Add = 1005,
        //Clone = 1006,
        //Delete = 1007,
        CSFViewer = IDC_CSF_VIEW_LIST,
        ItemDetailViewer = IDC_CSF_VIEW_RICH_EDT,
        //Save = 1011,
        SetLabel = IDC_CSF_VIEW_CUR_LABEL,
        Reload = IDC_CSF_VIEW_RELOAD,
        //Apply = 1016
    };

    using RowSearchHandler = std::function<bool(const CString&)>;

    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;
    virtual void OnOK() override;

    void translateUI();


    afx_msg void OnClose();
    afx_msg void onReload();
    void Update();
    void updateTextView();
    void displayCSFContent(const TranslationMap& csfMap, const RowSearchHandler handler);
    void FilterRows(const CString& searchText);
    void OnEditchangeSearch();
    void OnViewerSelectedChange();
    BOOL onMessageKeyDown(MSG* pMsg);

    //static BOOL CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    CRichEditCtrl m_richEditCtrl;
    CListCtrl m_stringList;
    CString CurrentSelectedCSF;
    CEdit m_selectedLabel;
    CEdit m_searchEdit;
};

