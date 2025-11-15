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

// NewMapImportDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "finalsun.h"
#include "NewMapImportDlg.h"
#include "variables.h"
#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CNewMapImportDlg 


CNewMapImportDlg::CNewMapImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMapImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMapImportDlg)
	m_ImportFile = _T("");
	m_ImportOverlay = FALSE;
	m_ImportTrees = FALSE;
	m_ImportUnits = FALSE;
	//}}AFX_DATA_INIT
}


void CNewMapImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMapImportDlg)
	DDX_CBString(pDX, IDC_IMPORTFILE, m_ImportFile);
	DDX_Check(pDX, IDC_IMPORTOVERLAY, m_ImportOverlay);
	DDX_Check(pDX, IDC_IMPORTTREES, m_ImportTrees);
	DDX_Check(pDX, IDC_IMPORTUNITS, m_ImportUnits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMapImportDlg, CDialog)
	//{{AFX_MSG_MAP(CNewMapImportDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CNewMapImportDlg 

void CNewMapImportDlg::OnBrowse()
{
	UpdateData();

	//CComboBox* m_ImportFile=(CComboBox*)GetDlgItem(IDC_IMPORTFILE);
	auto const fileTypeInfo = TranslateStringACP("NewMapImportFileTypes");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, fileTypeInfo);

	char cuPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cuPath);
	dlg.m_ofn.lpstrInitialDir = cuPath;

	if (theApp.m_Options.TSExe.GetLength()) {
		dlg.m_ofn.lpstrInitialDir = theApp.m_Options.TSExe;
	}


	if (dlg.DoModal() == IDCANCEL) {
		return;
	}

	m_ImportFile = dlg.GetPathName();

	UpdateData(FALSE);
}

BOOL CNewMapImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	translateUI();

	CComboBox* m_ImportFile = (CComboBox*)GetDlgItem(IDC_IMPORTFILE);

	CString maps = CString(u8AppDataPath.c_str()) + "\\stdmaps\\*.mpr";
	CFileFind ff;

	if (ff.FindFile(maps)) {
		BOOL bFileAvailable = TRUE;
		while (bFileAvailable) {
			bFileAvailable = ff.FindNextFile();

			CString file = ff.GetFileName();
			m_ImportFile->AddString(file);
		}

		m_ImportFile->SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CNewMapImportDlg::translateUI()
{
	TranslateWindowCaption(*this, "NewMapImportCaption");
	TranslateDlgItem(*this, IDC_NEWMAPIMPORT_TXT_DSC, "NewMapImportDesc");
	TranslateDlgItem(*this, IDC_BROWSE, "NewMapImportBrowse");
	TranslateDlgItem(*this, IDC_IMPORTTREES, "NewMapImportImportTrees");
	TranslateDlgItem(*this, IDC_IMPORTUNITS, "NewMapImportImportUnits");
	TranslateDlgItem(*this, IDC_IMPORTOVERLAY, "NewMapImportImportOverlay");

	SetDlgItemText(IDOK, GetLanguageStringACP("NewMapTypeOK"));
	SetDlgItemText(IDCANCEL, GetLanguageStringACP("Cancel"));
}
