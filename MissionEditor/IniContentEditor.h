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
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "res/resource.h"

class CIniContentEditor : public CDialog
{
public:
	enum { IDD = IDD_INI_EDITOR };

	CIniContentEditor(CWnd* pParent = nullptr);
	~CIniContentEditor() = default;

	void SetSection(const CString& val) { m_sectionID = val; }
	void SetContent(const CString& val) { m_content = val; }
	CString Content() const { return m_content; }

	CIniFileSection&& PopSection() { return std::move(m_section); }

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual void OnOK() override;

	void translateUI();
	BOOL onMessageKeyDown(MSG* pMsg);

	afx_msg void onSectionChanged();

	DECLARE_MESSAGE_MAP()

protected:
	CString m_sectionID;
	CString m_content;
	CIniFileSection m_section;

	CEdit m_sectionValue;
	CRichEditCtrl m_contentText;
};