﻿/*
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

#if !defined(AFX_NEWMAPSPDLG_H__F7D62445_C6DF_11D4_9C87_E63AC3E34349__INCLUDED_)
#define AFX_NEWMAPSPDLG_H__F7D62445_C6DF_11D4_9C87_E63AC3E34349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewMapSpDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CNewMapSpDlg 

class CNewMapSpDlg : public CDialog
{
	// Konstruktion
public:
	CNewMapSpDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CNewMapSpDlg)
	enum { IDD = IDD_NEWMAPSPOPTIONS };
	BOOL	m_AutoProd;
	int		m_House;
	BOOL	m_PrepareHouses;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CNewMapSpDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CNewMapSpDlg)
	afx_msg void OnPreparehouses();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_NEWMAPSPDLG_H__F7D62445_C6DF_11D4_9C87_E63AC3E34349__INCLUDED_
