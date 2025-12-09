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

// Tags.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FinalSun.h"
#include "Tags.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"
#include "TriggerDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CTags 

IMPLEMENT_DYNCREATE(CTags, CDialog)

CTags::CTags() : CDialog(CTags::IDD)
{
	//{{AFX_DATA_INIT(CTags)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}

CTags::~CTags()
{
}

BOOL CTags::OnInitDialog()
{
	auto const ret = CDialog::OnInitDialog();
	translateUI();
	return ret;
}

void CTags::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTags)
	DDX_Control(pDX, IDC_REPEAT, m_Repeat);
	DDX_Control(pDX, IDC_TAG, m_Tag);
	DDX_Control(pDX, IDC_TRIGGER, m_Trigger);
	DDX_Text(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}

BOOL CTags::PreTranslateMessage(MSG* pMsg)
{
	int ret = -1;
	if (pMsg->message == WM_KEYDOWN) {
		ret = onMessageKeyDown(pMsg);
	}

	return ret < 0 ? this->CDialog::PreTranslateMessage(pMsg) : ret;
}

BOOL CTags::onMessageKeyDown(MSG* pMsg)
{
	switch (pMsg->wParam) {
	default:
		return -1;
		case VK_RETURN:
		{
			switch (::GetDlgCtrlID(pMsg->hwnd)) {
			default:
				break;// never exist window (default -1) even nothing did
			}
		}
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CTags, CDialog)
	//{{AFX_MSG_MAP(CTags)
	ON_CBN_SELCHANGE(IDC_TAG, OnSelchangeTag)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_CBN_EDITCHANGE(IDC_REPEAT, OnEditchangeRepeat)
	ON_CBN_SELCHANGE(IDC_REPEAT, OnSelchangeRepeat)
	ON_CBN_EDITCHANGE(IDC_TRIGGER, OnEditchangeTrigger)
	ON_CBN_SELCHANGE(IDC_TRIGGER, OnSelchangeTrigger)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTags 

void CTags::UpdateDialog()
{
	CIniFile& ini = Map->GetIniFile();

	int initialSel = m_Tag.GetCurSel();

	while (m_Tag.DeleteString(0) != CB_ERR);
	while (m_Trigger.DeleteString(0) != CB_ERR);

	int i;
	auto const& tagDb = TagDatabase::Instance();
	for (auto const& tag : tagDb) {
		CString s;
		s.Format("%s (%s)", tag.id, tag.name);
		m_Tag.AddString(s);
	}

	auto const& triggerDb = TriggerDatabase::Instance();
	for (auto const& trigger: triggerDb) {
		CString s;
		s.Format("%s (%s)", trigger.ID(), trigger.Options().name);
		m_Trigger.AddString(s);
	}

	if (m_Tag.SetCurSel(0) != CB_ERR) {
		OnSelchangeTag();
	}

	if (initialSel != CB_ERR) {
		if (m_Tag.SetCurSel(initialSel) != CB_ERR) {
			OnSelchangeTag();
		}
	}

}

void CTags::translateUI()
{
	TranslateWindowCaption(*this, "TagsCaption");
	TranslateDlgItem(*this, IDC_DESC, "TagsDesc");
	TranslateDlgItem(*this, IDC_TAGS_CUR_TXT, "TagsCurrent");
	TranslateDlgItem(*this, IDC_ADD, "TagsAdd");
	TranslateDlgItem(*this, IDC_DELETE, "TagsDelete");
	TranslateDlgItem(*this, IDC_TAGS_NAME, "TagsName");
	TranslateDlgItem(*this, IDC_TAGS_REPEAT, "TagsRepeat");
	TranslateDlgItem(*this, IDC_TAGS_TRIGGER, "TagsTrigger");
	TranslateDlgItem(*this, IDC_TAGS_DESC_2, "TagsExpaination");

	m_Repeat.InsertString(0, TranslateStringACP("0 - Make the trigger work only once"));
	m_Repeat.InsertString(1, TranslateStringACP("1 - All entities attached"));
	m_Repeat.InsertString(2, TranslateStringACP("2 - Make the trigger repeating"));
}

void CTags::OnSelchangeTag()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}
	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	auto const& tagDb = TagDatabase::Instance();
	auto const& data = tagDb.Lookup(type);
	m_Name = data.name;
	auto const& triggerId = data.triggerId;
	CString desc = triggerId;

	auto const& triggerDb = TriggerDatabase::Instance();

	desc.Format("%s (%s)", triggerId, triggerDb.Lookup(triggerId).Options().name);

	m_Trigger.SetWindowText(desc);
	m_Repeat.SetWindowText(data.PersistenceString());

	UpdateData(FALSE);
}

void CTags::OnChangeName()
{
	CIniFile& ini = Map->GetIniFile();

	UpdateData();

	CEdit& name = *(CEdit*)GetDlgItem(IDC_NAME);
	int sel2 = name.GetSel();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}
	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	auto& data = TagDatabase::Instance().Lookup(type);
	data.name = m_Name;

	UpdateDialog();
	name.SetSel(sel2);
}

void CTags::OnEditchangeRepeat()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}

	CString str;
	m_Repeat.GetWindowText(str);
	str.Trim();

	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	auto& data = TagDatabase::Instance().Lookup(type);
	data.persistence = atoi(str); // TODO: validate str

	UpdateDialog();

}

void CTags::OnSelchangeRepeat()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}
	int v = m_Repeat.GetCurSel();
	CString str;

	m_Repeat.GetLBText(v, str);

	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	TruncSpace(str);

	auto& data = TagDatabase::Instance().Lookup(type);
	data.persistence = atoi(str); // TODO: validate str

	UpdateDialog();
}

void CTags::OnEditchangeTrigger()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) return;

	CString str;
	m_Trigger.GetWindowText(str);
	TruncSpace(str);

	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	auto& data = TagDatabase::Instance().Lookup(type);
	data.triggerId = str; // TODO: validate str
	
	//UpdateDialog();
}

void CTags::OnSelchangeTrigger()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}

	int v = m_Trigger.GetCurSel();
	CString str;


	m_Trigger.GetLBText(v, str);
	TruncSpace(str);

	CString type;
	m_Tag.GetLBText(index, type);
	TruncSpace(type);

	auto& data = TagDatabase::Instance().Lookup(type);
	data.triggerId = str; // TODO: validate str

	//UpdateDialog();
}

void CTags::OnDelete()
{
	CIniFile& ini = Map->GetIniFile();

	int index = m_Tag.GetCurSel();
	if (index < 0) {
		return;
	}

	int res = MessageBox(TranslateStringACP("TagDeleteConfirm"), TranslateStringACP("TagDeletion"), MB_YESNO);
	if (res == IDNO) {
		return;
	}

	TagDatabase::Instance().DeleteAt(index);

	UpdateDialog();
}

void CTags::OnAdd()
{
	auto const& triggerDb = TriggerDatabase::Instance();
	if (triggerDb.Size() <= 0) {
		MessageBox(TranslateStringACP("TagCreateWarning"), TranslateStringACP("Error"));
		return;
	};

	auto& tag = TagDatabase::Instance().
		Append(GetFreeID(), "New Tag");
	tag.triggerId = triggerDb.Nth(0).ID();

	UpdateDialog();

	int i;
	for (i = 0; i < m_Tag.GetCount(); i++) {
		CString tagID;
		m_Tag.GetLBText(i, tagID);
		TruncSpace(tagID);

		if (tagID == tag.id) {
			m_Tag.SetCurSel(i);
			break;
		}
	}
	OnSelchangeTag();
}
