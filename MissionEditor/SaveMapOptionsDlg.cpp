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

// SaveMapOptionsDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "finalsun.h"
#include "SaveMapOptionsDlg.h"
#include "variables.h"
#include "functions.h"
#include "inifile.h"
#include "res/resource.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSaveMapOptionsDlg 
CSaveMapOptionsDlg::CSaveMapOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveMapOptionsDlg::IDD, pParent)
{
	CIniFile& ini = Map->GetIniFile();

	m_Compress = 1;
	m_PreviewMode = PREVIEW_MINIMAP;
	auto const defMinPlayers = g_data.GetInteger("Customizations", "DefaultMinPlayers");
	m_MinPlayers = ini.GetInteger("Basic", "MinPlayer", defMinPlayers);
	m_MapName = ini.GetString("Basic", "Name");

	if (!Map->IsMultiplayer()) {
		m_PreviewMode = PREVIEW_DONT_CHANGE;
	}
}


void CSaveMapOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_PREVIEWMODE, reinterpret_cast<int&>(m_PreviewMode));
	DDX_Text(pDX, IDC_MAPNAME, m_MapName);
	DDX_Text(pDX, IDC_SAVE_OPT_M_PLAYERS, m_MinPlayers);
	DDX_Control(pDX, IDC_SAV_OPT_DLG_MODE_LIST, m_modeList);
}


BEGIN_MESSAGE_MAP(CSaveMapOptionsDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSaveMapOptionsDlg 

BOOL CSaveMapOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	translateUI();

	CIniFile& ini = Map->GetIniFile();
	if (!Map->IsMultiplayer()) {
		GetDlgItem(IDC_PREVIEWMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_NOPREVIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EXISTINGPREVIEW)->EnableWindow(FALSE);
#ifndef TS_MODE
		m_modeList.EnableWindow(FALSE);
#endif
		return TRUE;
	}

	initializeModeList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CSaveMapOptionsDlg::translateUI()
{


	//IDC_SAVE_OPT_MP_TXT;
}

void CSaveMapOptionsDlg::initializeModeList()
{
	m_modeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_modeList.InsertColumn(0, TranslateStringACP("CSaveMapOptionsModeName"), LVCFMT_LEFT, 125);
	m_modeList.InsertColumn(1, TranslateStringACP("CSaveMapOptionsModeID"), LVCFMT_LEFT, 80);

	auto const gameModeSec = g_data.GetSection("GameModes");
	auto const defMode = g_data.GetStringOr("Customizations", "DefaultGameMode", "standard");
	if (gameModeSec.Size() == 0) {
		m_modeList.InsertItem(0, defMode);
		return;
	}

	for (auto& mode : m_modes) {
		TruncSpace(mode);
	}
	
	for (auto idx = 0; idx < gameModeSec.Size();++idx) {
		auto const [mode, name] = gameModeSec.Nth(idx);
		auto modeCpy = mode;
		TruncSpace(modeCpy);
		m_modeList.InsertItem(idx, name);
		m_modeList.SetItemText(idx, 1, modeCpy);
		if (m_modes.empty() && mode == defMode
			|| std::find(m_modes.begin(), m_modes.end(), modeCpy) != m_modes.end()) {
			m_modeList.SetCheck(idx);
		}
	}
}

void CSaveMapOptionsDlg::OnOK()
{
	auto const itemCount = m_modeList.GetItemCount();
	std::set<CString> dedupSet;
	m_modes.clear();
	for (int idx = 0; idx < itemCount; ++idx) {
		if (m_modeList.GetCheck(idx)) {
			auto mode = m_modeList.GetItemText(idx, 1);
			if (dedupSet.emplace(mode).second) {
				m_modes.push_back(mode);
			}
		}
	}

	EndDialog(IDOK);
}