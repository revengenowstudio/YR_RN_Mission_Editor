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

#if !defined(AFX_TASKFORCE_H__58AF6EC0_92E3_11D3_B63B_85DE97E7FB41__INCLUDED_)
#define AFX_TASKFORCE_H__58AF6EC0_92E3_11D3_B63B_85DE97E7FB41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskForce.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CTaskForce 

class CTaskForce : public CDialog
{
	DECLARE_DYNCREATE(CTaskForce)

	// Konstruktion
public:
	void UpdateDialog();
	CTaskForce();
	~CTaskForce();

	// Dialogfelddaten
		//{{AFX_DATA(CTaskForce)
	enum { IDD = IDD_TASKFORCES };
	CComboBox	m_UnitType;
	CListBox	m_Units;
	CComboBox	m_TaskForces;
	CString	m_Group;
	CString	m_Name;
	int		m_NumberOfUnits;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CTaskForce)
protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	//}}AFX_VIRTUAL

// Implementierung
	CString getCurrrentID();
	void translateUI();
	void addTaskforce(CString&& name, int group, std::vector<CString>&& members);

	DECLARE_MESSAGE_MAP()

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CTaskForce)
	afx_msg void OnEditchangeTaskforces();
	afx_msg void OnSelchangeTaskforces();
	afx_msg void OnSelchangeUnits();
	afx_msg void OnDeleteunit();
	afx_msg void OnChangeNumberunits();
	afx_msg void OnChangeName();
	afx_msg void OnEditchangeUnittype();
	afx_msg void OnSelchangeUnittype();
	afx_msg void OnAddunit();
	afx_msg void OnBnClickedCopyunit();
	afx_msg void OnDeletetaskforce();
	afx_msg void OnAddtaskforce();
	afx_msg void OnBnClickedCopytaskforce();
	afx_msg void OnChangeGroup();
	//}}AFX_MSG
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TASKFORCE_H__58AF6EC0_92E3_11D3_B63B_85DE97E7FB41__INCLUDED_
