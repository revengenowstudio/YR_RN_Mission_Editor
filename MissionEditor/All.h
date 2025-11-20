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

#if !defined(AFX_ALL1_H__B5D522E4_69CE_11D3_99E1_C138647F2A00__INCLUDED_)
#define AFX_ALL1_H__B5D522E4_69CE_11D3_99E1_C138647F2A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// All1.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAll 

class CAll : public CDialog
{
	// Konstruktion
public:	
	enum { IDD = IDD_ALL };

	void UpdateDialog();
	CAll(CWnd* pParent = NULL);   // Standardkonstruktor

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog() override;
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	//}}AFX_VIRTUAL

// Implementierung
	void translateUI();
	BOOL onMessageKeyDown(MSG* pMsg);

	afx_msg void OnSelchangeSections();
	afx_msg void OnSearchEditChange();
	afx_msg void OnSearchApply();
	afx_msg void OnAddSection();
	afx_msg void OnDeleteSection();
	afx_msg void OnEditSection();
	afx_msg void OnIniSectionImport();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	static constexpr int TIMER_IDX_SEARCH = 1;
	static constexpr int SEARCH_DELAY_MS = 1000;

	CEdit			m_SearchString;
	CRichEditCtrl	m_Value;
	CListBox		m_Sections;
	CButton			m_IniSection;
	CButton			m_AddSection;
	CButton			m_DeleteSection;
	CButton			m_EditButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ALL1_H__B5D522E4_69CE_11D3_99E1_C138647F2A00__INCLUDED_
