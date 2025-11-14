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

#if !defined(AFX_SAVEMAPOPTIONSDLG_H__DD7874E1_CED2_11D4_9C87_444553540000__INCLUDED_)
#define AFX_SAVEMAPOPTIONSDLG_H__DD7874E1_CED2_11D4_9C87_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveMapOptionsDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSaveMapOptionsDlg 

class CSaveMapOptionsDlg : public CDialog
{
public:
	enum PreviewType {
		PREVIEW_MINIMAP,
		PREVIEW_DONT_CHANGE,
		PREVIEW_STRIP,
	};

	// Konstruktion
	CSaveMapOptionsDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CSaveMapOptionsDlg)
	enum { IDD = IDD_SAVEOPTIONS };
	int				m_Compress;
	PreviewType		m_PreviewMode;
	int				m_MinPlayers;
	CString			m_MapName;

	std::vector<CString> m_modes;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CSaveMapOptionsDlg)
protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void translateUI();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSaveMapOptionsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CListCtrl m_modeList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_SAVEMAPOPTIONSDLG_H__DD7874E1_CED2_11D4_9C87_444553540000__INCLUDED_
