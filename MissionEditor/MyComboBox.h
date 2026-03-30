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

#if !defined(AFX_MYCOMBOBOX_H__6B1D6F02_A9A7_11D3_B63B_B4DF98412640__INCLUDED_)
#define AFX_MYCOMBOBOX_H__6B1D6F02_A9A7_11D3_B63B_B4DF98412640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyComboBox.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CMyComboBox 

class CMyComboBox : public CComboBox
{
public:
	// Konstruktion
	CMyComboBox();

	// Attribute

	// Operationen

	// Implementierung
	virtual ~CMyComboBox();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:

	/**
	* This will automatically send a CBN_EDITCHANGE command.
	* Also it will set the window text to the selected item.
	* So you MUST bind it to ON_CBN_EDITCHANGE instead of ON_CBN_SELCHANGE
	*/
	afx_msg void OnSelchange();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_MYCOMBOBOX_H__6B1D6F02_A9A7_11D3_B63B_B4DF98412640__INCLUDED_
