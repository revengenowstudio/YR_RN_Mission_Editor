#include "stdafx.h"
#include "CsfViewer.h"
#include "variables.h"
#include "functions.h"

BEGIN_MESSAGE_MAP(CCsfViewer, CDialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(Controls::Reload, onReload)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_CSF_VIEW_LIST, &CCsfViewer::OnViewerSelectedChange)
END_MESSAGE_MAP()

#if 0
LRESULT CALLBACK CCsfViewer::ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //return DarkTheme::MyCallWindowProcA(g_pOriginalListViewProc, hWnd, uMsg, wParam, lParam);
}
#endif

CCsfViewer::CCsfViewer(CWnd* pParent) : CDialog(CCsfViewer::IDD, pParent)
{
}
CCsfViewer::~CCsfViewer()
{
}

BOOL CCsfViewer::OnInitDialog()
{
    if (!CDialog::OnInitDialog()) {
        return FALSE;
    }

    translateUI();

    Reset();
    onReload();

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CCsfViewer::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, Controls::CSFViewer, m_stringList);
    DDX_Control(pDX, Controls::ItemDetailViewer, m_richEditCtrl);
    DDX_Control(pDX, Controls::SetLabel, m_selectedLabel);
    DDX_Control(pDX, Controls::SelectedCSF, m_searchEdit);
}

void CCsfViewer::translateUI()
{
    SetWindowText(GetLanguageStringACP("CsfViewerTitle"));
    //GetDlgItem(1000)->SetWindowText(GetLanguageStringACP("CsfViewerSelectedCsfFile"));
    //GetDlgItem(1002)->SetWindowText(GetLanguageStringACP("CsfViewerNewFile"));
    GetDlgItem(1003)->SetWindowText(GetLanguageStringACP("CsfViewerSearchLabelText"));
    //GetDlgItem(1005)->SetWindowText(GetLanguageStringACP("CsfViewerAdd"));
    //GetDlgItem(1006)->SetWindowText(GetLanguageStringACP("CsfViewerClone"));
    //GetDlgItem(1007)->SetWindowText(GetLanguageStringACP("CsfViewerDelete"));
    GetDlgItem(1010)->SetWindowText(GetLanguageStringACP("CsfViewerDescription1"));
    //GetDlgItem(1014)->SetWindowText(GetLanguageStringACP("CsfViewerDescription2"));
    //GetDlgItem(1011)->SetWindowText(GetLanguageStringACP("CsfViewerSave"));
    GetDlgItem(1012)->SetWindowText(GetLanguageStringACP("CsfViewerSetLabelName"));
    GetDlgItem(Controls::Reload)->SetWindowText(GetLanguageStringACP("CsfViewerReload"));
    //GetDlgItem(1016)->SetWindowText(GetLanguageStringACP("CsfViewerApply"));

    SetDlgItemText(IDOK, GetLanguageStringACP("OK"));
    //SetDlgItemText(IDCANCEL, GetLanguageStringACP("Cancel")); // TODO: add cancel
    m_stringList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_richEditCtrl.SetReadOnly();

    //ExtraWindow::SetEditControlFontSize(m_richEditCtrl, 1.4f, true);
    //CFont font();
    //m_richEditCtrl.SetFont(CFont)

#if 0
    if (ExtConfigs::EnableDarkMode)
    {
        ::SendMessage(m_richEditCtrl, EM_SETBKGNDCOLOR, (WPARAM)FALSE, (LPARAM)RGB(32, 32, 32));
        CHARFORMAT cf = { 0 };
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = RGB(220, 220, 220);
        ::SendMessage(m_richEditCtrl, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
        ::SendMessage(hCSFViewer, LVM_SETTEXTBKCOLOR, 0, RGB(32, 32, 32));
        ::SendMessage(hCSFViewer, LVM_SETTEXTCOLOR, 0, RGB(220, 220, 220));

        DarkTheme::SubclassListViewHeader(hCSFViewer);
        g_pOriginalListViewProc = (WNDPROC)GetWindowLongPtr(hCSFViewer, GWLP_WNDPROC);
        if (g_pOriginalListViewProc)
        {
            SetWindowLongPtr(hCSFViewer, GWLP_WNDPROC, (LONG_PTR)ListViewSubclassProc);
        }
        InvalidateRect(hCSFViewer, NULL, TRUE);
    }

    Update(hWnd);
#endif
}

void CCsfViewer::OnClose()
{
    EndDialog(IDCANCEL);
    // reduce lag
    //EndDialog(hWnd, NULL);
    //ShowWindow(SW_HIDE);
}

void CCsfViewer::onReload()
{
    last_succeeded_operation = 9;
    //StringtableLoader::CSFFiles_Stringtable.clear();
    //StringtableLoader::LoadCSFFiles();
    //Logger::Debug("Successfully loaded %d csf labels.\n", StringtableLoader::CSFFiles_Stringtable.size());

    //char tmpCsfFile[0x400];
    //strcpy_s(tmpCsfFile, CFinalSunAppExt::ExePathExt);
    //strcat_s(tmpCsfFile, "\\RA2Tmp.csf");
    //DeleteFile(tmpCsfFile);

    // TODO: reload CSF file
    //theApp.m_loading->LoadStrings();

    //ExtraWindow::bEnterSearch = true;
    Update();
    //ExtraWindow::bEnterSearch = false;

    //UpdateDialog();
    ((CFinalSunDlg*)theApp.m_pMainWnd)->UpdateDialogs(TRUE);
}

void CCsfViewer::SetSelectedString(CString str)
{
    if (str == CurrentSelectedCSF) {
        return;
    }
    // TODO: reset dialog
}

void CCsfViewer::Update()
{
    displayCSFContent(AllStrings, [](const CString&) { return true; });
    m_richEditCtrl.SetWindowText("");
    m_selectedLabel.SetWindowText("");
    CurrentSelectedCSF = "";

    CString txt;
    m_searchEdit.GetWindowText(txt);
    if (!txt.IsEmpty()) {
        OnEditchangeSearch();
    }
    //NeedUpdate = false;
}

void CCsfViewer::updateTextView()
{
    if (CurrentSelectedCSF != "")
    {
        auto it = AllStrings.find(CurrentSelectedCSF);
        if (it != AllStrings.end())
        {
            int index = std::distance(AllStrings.begin(), it);

            LVITEM lvItem = { 0 };
            lvItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
            lvItem.state = LVIS_SELECTED | LVIS_FOCUSED;

            m_stringList.SetItemState(index, &lvItem);
            m_stringList.EnsureVisible(index, TRUE);

            m_stringList.SetFocus();
        }
    }
    CurrentSelectedCSF = "";
}

BOOL CCsfViewer::PreTranslateMessage(MSG* pMsg)
{
    int ret = -1;
    if (pMsg->message == WM_KEYDOWN) {
        ret = onMessageKeyDown(pMsg);
    }

    return ret < 0 ? this->CDialog::PreTranslateMessage(pMsg) : ret;
}

BOOL CCsfViewer::onMessageKeyDown(MSG* pMsg)
{
    switch (pMsg->wParam) {
    default:
        return -1;
    case VK_RETURN:
    {
        switch (::GetDlgCtrlID(pMsg->hwnd)) {
        default:
            break;// never exist window (default -1) even nothing did
        case Controls::Search: this->OnEditchangeSearch();
            break;
        }
    }
    }
    return TRUE;
}

#if 0
BOOL CALLBACK CCsfViewer::DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_NOTIFY:
    {
        LPNMHDR pNMHDR = (LPNMHDR)lParam;
        if (pNMHDR->idFrom == Controls::CSFViewer && pNMHDR->code == LVN_ITEMCHANGED)
        {
            OnViewerSelectedChange(pNMHDR);
        }
        else if (pNMHDR->idFrom == Controls::CSFViewer && pNMHDR->code == NM_DBLCLK)
        {
            OnClickApply();
        }
        break;
    }
    case WM_CLOSE:
    {
        CCsfViewer::Close(hWnd);
        return TRUE;
    }
    case 114514: // used for update
    {
        if (NeedUpdate)
            Update(hWnd);
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        return TRUE;
    }
    case 114515: // used for update
    {

        return TRUE;
    }
    case 114516: // used for update
    {
        Update(hWnd);
        return TRUE;
    }
    }

    // Process this message through default handler
    return FALSE;
}
#endif

void CCsfViewer::OnViewerSelectedChange(NMHDR* pNMHDR, LRESULT* pResult)
{
    int nSelected = m_stringList.GetNextItem(-1, LVNI_SELECTED);
    if (nSelected == -1) {
        return;
    }

    CString selectedText = m_stringList.GetItemText(nSelected, 0);

    if (selectedText.IsEmpty()) {
        m_richEditCtrl.SetWindowText("");
        m_selectedLabel.SetWindowText("");
        CurrentSelectedCSF = "";
        return;
    }

    CString value = "";
    auto const it = AllStrings.find(selectedText);
    if (it == AllStrings.end()) {
        return;
    }

    m_richEditCtrl.SetWindowText(it->second.cString);
    m_selectedLabel.SetWindowText(selectedText);

    CurrentSelectedCSF = selectedText;
    *pResult = 0;
}

void CCsfViewer::Reset()
{
    m_userConfirmed = false;
    onReload();
}

// TODO: make it MB_OK
void CCsfViewer::OnOK()
{
    if (CurrentSelectedCSF.IsEmpty()) {
        EndDialog(IDCANCEL);
        return;
    }
    EndDialog(IDOK);
}

void CCsfViewer::displayCSFContent(const TranslationMap& csfMap, const RowSearchHandler handler)
{
    m_stringList.DeleteAllItems();
    while (m_stringList.DeleteColumn(0));

    CString txt;
    txt = TranslateStringACP("CsfViewerColumnLabel");
    LVCOLUMN lvColumn = { 0 };
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.pszText = const_cast<LPSTR>(txt.operator LPCSTR());
    lvColumn.cx = 100;
    m_stringList.InsertColumn(0, &lvColumn);

    txt = TranslateStringACP("CsfViewerColumnText");
    lvColumn.cx = 400;
    lvColumn.pszText = const_cast<LPSTR>(txt.operator LPCSTR());

    m_stringList.InsertColumn(1, &lvColumn);

    LVITEM lvItem = { 0 };
    lvItem.mask = LVIF_TEXT;

    int i = 0;
    for (auto const& [label, content] : csfMap) {
        if (!handler(label) && !handler(content.cString)) {
            continue;
        }

        lvItem.mask = LVIF_TEXT;
        lvItem.iItem = i;
        lvItem.iSubItem = 0;
        lvItem.pszText = const_cast<LPSTR>(label.operator LPCSTR());
        m_stringList.InsertItem(&lvItem);

        m_stringList.SetItemText(i, 1, const_cast<LPSTR>(content.cString.operator LPCSTR()));

        i++;
    }

    m_stringList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}

void CCsfViewer::FilterRows(const CString& searchText)
{
    displayCSFContent(AllStrings, [&searchText](const CString& content) {
        return content.FindOneOf(searchText);
        });
}

void CCsfViewer::OnEditchangeSearch()
{
    //if (CCStrings.size() > ExtConfigs::SearchCombobox_MaxCount && !ExtraWindow::bEnterSearch)
    //{
    //    return;
    //}

    CString buffer;
    m_searchEdit.GetWindowText(buffer);

    displayCSFContent(AllStrings, [&buffer](const CString& content) {
        return buffer.IsEmpty() || content.Find(buffer) >= 0;
    });
    //NeedUpdate = false;
}
