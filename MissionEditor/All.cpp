/*
	FinalSun/FinalAlert 2 Mission Editor

	Copyright (C) 1999-2024 Electronic Arts, Inc.
	Authored by Matthias Wagner

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// All1.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FinalSun.h"
#include "All.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"
#include "IniContentEditor.h"
#include <sstream>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern CFinalSunApp theApp;
#include "ImportIni.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAll 


CAll::CAll(CWnd* pParent /*=NULL*/) : 
	CDialog(CAll::IDD, pParent),
	m_skipSearchOnce(false)
{
	//{{AFX_DATA_INIT(CAll)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INI_EDITOR_CONTENT, m_Value);
	DDX_Control(pDX, IDC_EDITOR_SECTIONS, m_Sections);
	DDX_Control(pDX, IDC_INISECTION, m_IniSection);
	DDX_Control(pDX, IDC_DELETESECTION, m_DeleteSection);
	DDX_Control(pDX, IDC_EDITOR_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_ADDSECTION, m_AddSection);
	DDX_Control(pDX, IDC_EDITOR_SEARCH, m_SearchString);
	DDX_Control(pDX, IDC_INI_E_SEARCH_CASED, m_Cased);
}

BOOL CAll::OnInitDialog()
{
	auto const ret = CDialog::OnInitDialog();

	translateUI();

	CFont font;
	font.CreatePointFont(100, _T("Tahoma"));
	m_Value.SetFont(&font);
	m_Value.SetReadOnly();

	m_IniSection.EnableWindow(FALSE); // disable import feature for now

	return ret;
}

void CAll::translateUI()
{
	TranslateWindowCaption(*this, "IniEditorCaption");

	TranslateDlgItem(*this, IDC_INI_EDITOR_DESC, "IniEditorDesc");
	TranslateDlgItem(*this, IDC_EDITOR_EDIT_BUTTON, "IniEditorEditSection");
	TranslateDlgItem(*this, IDC_INI_EDITOR_KEYS, "IniEditorSectionKeys");
	TranslateDlgItem(*this, IDC_INI_EDITOR_TXT_SEARCH, "IniEditorSearch");
	TranslateDlgItem(*this, IDC_INI_E_SEARCH_CASED, "IniEditorSearchCased");
	
	TranslateDlgItem(*this, IDC_ADDSECTION, "IniEditorAdd");
	TranslateDlgItem(*this, IDC_DELETESECTION, "IniEditorDelete");
	TranslateDlgItem(*this, IDC_INISECTION, "IniEditorInsert");
	
}

BEGIN_MESSAGE_MAP(CAll, CDialog)
	//{{AFX_MSG_MAP(CAll)
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_EDITOR_SECTIONS, OnSelChangeSections)
	ON_EN_CHANGE(IDC_EDITOR_SEARCH, OnSearchEditChange)
	ON_BN_CLICKED(IDC_INI_E_SEARCH_CASED, OnSearchEditChange)
	ON_BN_CLICKED(IDC_ADDSECTION, OnAddSection)
	ON_BN_CLICKED(IDC_DELETESECTION, OnDeleteSection)
	ON_BN_CLICKED(IDC_EDITOR_EDIT_BUTTON, OnEditSection)
	ON_BN_CLICKED(IDC_INISECTION, OnIniSectionImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CAll 

BOOL CAll::PreTranslateMessage(MSG* pMsg)
{
	int ret = -1;
	if (pMsg->message == WM_KEYDOWN) {
		ret = onMessageKeyDown(pMsg);
	}
	return ret < 0 ? this->CDialog::PreTranslateMessage(pMsg) : ret;
}

BOOL CAll::onMessageKeyDown(MSG* pMsg)
{
	switch (pMsg->wParam) {
	default:
		return -1;
	case VK_RETURN:
	{
		switch (::GetDlgCtrlID(pMsg->hwnd)) {
		default:
			break;// never exist window (default -1) even nothing did
			//case IDC_INI_E_CUR_SEC_VAL: this->onEditchangeSearch();
			//	break;
		}
	}
	}
	return TRUE;
}

void CAll::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_IDX_SEARCH) {
		KillTimer(TIMER_IDX_SEARCH);
		OnSearchApply();
	}
	CDialog::OnTimer(nIDEvent);
}

void CAll::UpdateDialog(CString selection)
{
	while (m_Sections.DeleteString(0) != -1);

	m_Value.SetWindowText("");
	m_SearchString.SetWindowText("");

	CIniFile& ini = Map->GetIniFile();
	for (auto const& [name, sec] : ini) {
		if (!Map->IsMapSection(name)) {
			m_Sections.InsertString(-1, name);
		}
	}

	int selectionIdx = 0;
	if (int index = m_Sections.FindString(-1, selection); index != LB_ERR) {
		selectionIdx = index;
	}
	m_Sections.SetCurSel(selectionIdx);
	OnSelChangeSections();
}

void CAll::OnSelChangeSections()
{
	CIniFile& ini = Map->GetIniFile();

	CString curSection;
	if (auto const selected = m_Sections.GetCurSel(); selected >= 0) {
		m_Sections.GetText(selected, curSection);
	}

	if (curSection.GetLength()) {
		std::stringstream sectionItems;
		for (auto const& [key, val] : ini[curSection]) {
			sectionItems << key << '=' << val << std::endl;
		}
		auto const itemString = sectionItems.str();
		m_Value.SetWindowText(itemString.c_str());
	}
}

void CAll::OnEditSection()
{
	CString curSection;
	if (auto const selected = m_Sections.GetCurSel(); selected >= 0) {
		m_Sections.GetText(selected, curSection);
	}
	if (curSection.IsEmpty()) {
		return;
	}

	CIniContentEditor innerEditor;

	innerEditor.SetSection(curSection);
	{
		CString curItems;
		m_Value.GetWindowText(curItems);
		innerEditor.SetContent(curItems);
	}

	if (innerEditor.DoModal() == IDCANCEL) {
		return;
	}

	m_Value.SetWindowText(innerEditor.Content());

	auto& ini = Map->GetIniFile();
	ini.AddSection(curSection) = innerEditor.PopSection();
}

void CAll::OnAddSection()
{
	CString name = InputBox(GetLanguageStringACP("IniEditorAddTip"), GetLanguageStringACP("IniEditorAdd"));

	name.Trim();

	if (name.IsEmpty()) {
		return;
	}

	if (Map->IsMapSection(name)) {
		MessageBox(GetLanguageStringACP("IniEditorAddNotAllowed"), GetLanguageStringACP("Error"), MB_OK);
		return;
	}

	CIniFile& ini = Map->GetIniFile();

	ini.AddSection(name);

	m_skipSearchOnce = true;
	UpdateDialog(name);
}

void CAll::OnSearchEditChange()
{
	// search string set will trigger OnSearchEditChange again
	if (std::exchange(m_skipSearchOnce, false)) {
		return;
	}
	// Control tick
	SetTimer(TIMER_IDX_SEARCH, SEARCH_DELAY_MS, NULL);
}

void CAll::OnSearchApply()
{
	CString searchString;
	m_SearchString.GetWindowText(searchString);

	if (searchString.IsEmpty()) {
		m_skipSearchOnce = true;
		UpdateDialog();
		return;
	}

	try {
		// TODO: optional ECMAScript
		std::regex_constants::syntax_option_type regexOpts{ std::regex_constants::ECMAScript };
		if (m_Cased.GetCheck() != BST_CHECKED) {
			regexOpts |= std::regex_constants::icase;
		}

		auto const& ini = Map->GetIniFile();
		std::regex rule(searchString.operator LPCSTR(), regexOpts);
		static std::vector<CIniFile::Const_It> results; // hold buffer memory, do not reallocate frequently
		results.clear();
		results.reserve(ini.Size());
		for (auto it = ini.begin(); it != ini.end(); ++it) {
			if (Map->IsMapSection(it->first)) {
				continue;
			}
			if (std::regex_search(it->first.operator LPCSTR(), rule)) {
				results.emplace_back(it);
			}
		}

		while (m_Sections.DeleteString(0) != -1);
		m_Value.SetWindowText("");

		for (auto const& it : results) {		
			m_Sections.InsertString(-1, it->first);
		}

		if (m_Sections.GetCount() > 0) {
			m_Sections.SetCurSel(0);
			OnSelChangeSections();
		}

	} catch (const std::regex_error& e) {
		errstream << "Regex error: " << e.what() << std::endl;
		MessageBox(GetLanguageStringACP("IniEditorRegexError"), GetLanguageStringACP("Error"), MB_OK);
		return;
	}

}

void CAll::OnDeleteSection()
{
	CIniFile& ini = Map->GetIniFile();

	const int cusection = m_Sections.GetCurSel();

	if (cusection == -1) {
		auto const msg = TranslateStringACP("IniEditorItemUnselected");
		auto const cap = TranslateStringACP("Error");
		MessageBox(msg, cap);
		return;
	}

	CString str;
	m_Sections.GetText(cusection, str);

	auto const msgBefore = TranslateStringACP("IniEditorSelectionDeletePrefix");
	auto const msgAfter = TranslateStringACP("IniEditorSelectionDeleteSuffix");
	auto const cap = TranslateStringACP("IniEditorDeleteSelectionCap");

	if (MessageBox(msgBefore + str + msgAfter, cap, MB_YESNO) == IDNO) {
		return;
	}

	ini.DeleteSection(str);

	UpdateDialog();
}

void CAll::OnIniSectionImport()
{
	CFileDialog dlg(FALSE, ".ini", "*.ini", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "INI files|*.ini|");

	char cuPath[MAX_PATH];
	BOOL hidePreview = FALSE;
	BOOL previewPrinted = FALSE;
	GetCurrentDirectory(MAX_PATH, cuPath);
	dlg.m_ofn.lpstrInitialDir = cuPath;

	if (theApp.m_Options.TSExe.GetLength()) {
		dlg.m_ofn.lpstrInitialDir = theApp.m_Options.TSExe;
	}

	if (dlg.DoModal() != IDCANCEL) {
		CImportINI impini;
		impini.m_FileName = dlg.GetPathName();
		if (impini.DoModal() != IDCANCEL) {
			UpdateDialog();
		}
	}
}
