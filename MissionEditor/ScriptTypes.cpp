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

// ScriptTypes.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FinalSun.h"
#include "ScriptTypes.h"
#include "FinalSunDlg.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CScriptTypes 

IMPLEMENT_DYNCREATE(CScriptTypes, CDialog)

int scriptTypeIndexToComboBoxIndex(CComboBox& comboBox, int scriptTypeIndex)
{
	auto const totalCount = comboBox.GetCount();
	for (auto idx = 0; idx < totalCount; ++idx) {
		const int MData = comboBox.GetItemData(idx);
		if (MData == scriptTypeIndex) {
			return idx;
		}
	}

	return 0;
}
ExtraParameterType getExtraParamType(ParameterType paramType)
{
	switch (paramType) {
		default:
			return ExtraParameterType::None;
		case PRM_BuildingType:
			return ExtraParameterType::ScanType;
	}
}

std::vector<CString> ScriptTemplate::parse(const CString& content)
{
	auto const init = INIHelper::Split(content);
	auto const count = std::min<size_t>(atoi(init[0]), init.size() / 2);
	return parse(init, count, 1);
}

std::vector<CString> ScriptTemplate::parse(const std::vector<CString>& content, size_t count, size_t offset)
{
	std::vector<CString> ret;
	ret.reserve(count);
	for (auto i = 0; i < count; i++) {
		ret.emplace_back(content[static_cast<size_t>(2 * i) + offset]
			+ ','
			+ content[static_cast<size_t>(2 * i) + 1 + offset]);
	}
	return ret;
}

CScriptTypes::CScriptTypes() : CDialog(CScriptTypes::IDD)
{
	//{{AFX_DATA_INIT(CScriptTypes)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}

CScriptTypes::~CScriptTypes()
{
}

void CScriptTypes::UpdateStrings()
{
	TranslateDlgItem(*this, IDC_ADD, "ScriptTypesAddScript");
	TranslateDlgItem(*this, IDC_SCRIPT_COPY, "ScriptTypesCopyScript");
	TranslateDlgItem(*this, IDC_DELETE, "ScriptTypesDelScript");

	TranslateDlgItem(*this, IDC_SCTIPTTYPE_INRO, "ScriptTypesDesc");
	TranslateDlgItem(*this, IDC_SCRIPT_TEMPLATE_DSC, "ScriptTypesSelectedTemplate");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_TYPE, "ScriptTypesSelectedScript");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_NAME, "ScriptTypesName");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_ACTIONS, "ScriptTypesActions");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_ACTIONTYPE, "ScriptTypesActionType");
	TranslateDlgItem(*this, IDC_PDESC, "ScriptTypesActionParam");
	TranslateDlgItem(*this, IDC_SCRIPT_EXDESC, "ScriptTypesParamExtDesc");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_DESC, "ScriptTypesActionDesc");
	TranslateDlgItem(*this, IDC_ADDACTION, "ScriptTypesAddAction");
	TranslateDlgItem(*this, IDC_DELETEACTION, "ScriptTypesDelAction");

	TranslateWindowCaption(*this, "ScriptsCaption");
}

void CScriptTypes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptTypes)
	DDX_Control(pDX, IDC_DESCRIPTION, m_Description);
	DDX_Control(pDX, IDC_PDESC, m_ParamDesc);
	DDX_Control(pDX, IDC_TYPE, m_ActionType);
	DDX_Control(pDX, IDC_SCRIPTTYPE, m_ScriptType);
	DDX_Control(pDX, IDC_SCRIPT_TEMPLATE, m_Template);
	DDX_Control(pDX, IDC_PARAM, m_Param);
	DDX_Control(pDX, IDC_SCRIPT_EXTRA, m_ParamExt);
	DDX_Control(pDX, IDC_ACTION, m_Actions);
	DDX_Text(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptTypes, CDialog)
	ON_CBN_SELCHANGE(IDC_SCRIPTTYPE, OnSelchangeScripttype)
	ON_LBN_SELCHANGE(IDC_ACTION, OnSelchangeActionList)
	ON_EN_KILLFOCUS(IDC_NAME, OnChangeName)
	ON_CBN_KILLFOCUS(IDC_TYPE, OnEditchangeActionType)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeActionType)
	ON_CBN_KILLFOCUS(IDC_PARAM, OnEditchangeParam)
	ON_CBN_SELCHANGE(IDC_PARAM, OnSelchangeParam)
	ON_BN_CLICKED(IDC_ADDACTION, OnAddaction)
	ON_BN_CLICKED(IDC_DELETEACTION, OnDeleteaction)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELCHANGE(IDC_SCRIPT_EXTRA, &CScriptTypes::OnCbnSelchangeScriptExtra)
	ON_CBN_SELCHANGE(IDC_SCRIPT_TEMPLATE, &CScriptTypes::OnCbnSelchangeScriptTemplate)
	ON_BN_CLICKED(IDC_SCRIPT_COPY, &CScriptTypes::OnBnClickedScriptCopy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CScriptTypes 

void CScriptTypes::reloadTemplates()
{
	auto const& sec = g_data["ScriptTemplates"];
	auto const count = sec.GetInteger("Counts");
	auto const defName = sec.GetStringOr("DefaultName", "Default");
	m_scriptTemplates.push_back(
		ScriptTemplate{ defName, "New Script", "1,0,0" }
	);

	auto offset = 0;

	auto parseTemplate = [this, &sec](const CString& id) -> bool {
		auto const& record = sec.GetString(id);
		if (record.IsEmpty()) {
			return false;
		}
		auto&& parts = INIHelper::Split(record, ',');
		if (parts.size() <= 3) {
			return false;
		}
		auto const contentCount = atoi(parts[2]);
		if (contentCount * 2 != parts.size() - 3) {
			errstream << "record count mismatches content size" << std::endl;
			return false;
		}
		std::vector<CString> remaining(parts.begin() + 3, parts.end());
		this->m_scriptTemplates.emplace_back(ScriptTemplate{ parts[0], parts[1], remaining });
		return true;
	};

	// try from 0. If no 0, try from 1
	if (!parseTemplate("0")) {
		errstream << "[script template] could not parse content from index 0" << std::endl;
		offset = 1;
	}

	CString idStr;
	for (auto idx = 0; idx < count; ++idx) {
		idStr.Format("%d", idx + offset);
		if (!parseTemplate(idStr)) {
			errstream << "[script template] could not parse content from index " << idStr << std::endl;
		}
	}

	// load template into list
	ASSERT(m_scriptTemplates.size() > 0);
	for (auto const& templ : m_scriptTemplates) {
		m_Template.AddString(templ.Desc());
	}
	m_Template.SetCurSel(0);
}

void CScriptTypes::UpdateDialog()
{
	if (!m_scriptTemplates.size()) {
		reloadTemplates();
	}

	CIniFile& ini = Map->GetIniFile();

	int lastScriptSel = m_ScriptType.GetCurSel();


	while (m_ScriptType.DeleteString(0) != CB_ERR);


	// MW 07/24/01: clear dialog
	m_Description.SetWindowText("");
	m_Name = "";
	m_Param.SetWindowText("");
	m_ParamExt.SetWindowText("");
	m_Actions.SetWindowText("");
	m_ActionType.SetCurSel(-1);

	UpdateData(FALSE);

	// load script type into list
	for (auto const& [seq, type] : ini["ScriptTypes"]) {
		CString desc = type;
		desc += " (";
		desc += ini.GetString(type, "Name");
		desc += ")";
		m_ScriptType.AddString(desc);
	}

	m_ScriptType.SetCurSel(0);
	if (lastScriptSel >= 0) {
		m_ScriptType.SetCurSel(lastScriptSel);
	}
	OnSelchangeScripttype();


}

void CScriptTypes::OnSelchangeScripttype()
{
	CIniFile& ini = Map->GetIniFile();

	int sel = m_Actions.GetCurSel();
	while (m_Actions.DeleteString(0) != CB_ERR);

	CString Scripttype;
	if (m_ScriptType.GetCurSel() < 0) {
		return;
	}
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);

	m_Name = ini.GetString(Scripttype, "Name");

	int count = ini[Scripttype].Size() - 1;
	int i;
	for (i = 0; i < count; i++) {
		char c[50];
		itoa(i, c, 10);
		m_Actions.AddString(c);
	}


	m_Actions.SetCurSel(0);
	if (sel >= 0) {
		m_Actions.SetCurSel(sel);
	}
	OnSelchangeActionList();

	UpdateData(FALSE);
}

void CScriptTypes::OnSelchangeActionList()
{
	auto& doc = Map->GetIniFile();
	CString scriptId, buffer, paramNumStr;
	int listIndex, actionIndex, selectIndex;

	auto const scriptIndex = this->m_ScriptType.GetCurSel();
	listIndex = this->m_Actions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->m_ScriptType.GetLBText(scriptIndex, scriptId);
		TruncSpace(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetStringOr(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex < 0) {
			buffer += ",0";
			actionIndex = buffer.GetLength() - 2;
		}
		paramNumStr = buffer.Mid(actionIndex + 1);
		TruncSpace(paramNumStr);

		actionIndex = atoi(buffer.Mid(0, actionIndex));

		selectIndex = scriptTypeIndexToComboBoxIndex(this->m_ActionType, actionIndex);

		this->m_ActionType.SetCurSel(selectIndex);
		this->UpdateParams(actionIndex, &paramNumStr);
		this->m_Param.SetWindowText(paramNumStr);
	}
}

void CScriptTypes::OnChangeName()
{
	CIniFile& ini = Map->GetIniFile();

	UpdateData();

	CEdit* n = (CEdit*)GetDlgItem(IDC_NAME);

	DWORD pos = n->GetSel();
	CString Scripttype;
	if (m_ScriptType.GetCurSel() < 0) {
		return;
	}
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);



	ini.SetString(Scripttype, "Name", m_Name);

	UpdateDialog();
	n->SetSel(pos);
}

void CScriptTypes::OnEditchangeActionType()
{
	CIniFile& doc = Map->GetIniFile();

	const int scriptIndex = this->m_ScriptType.GetCurSel();
	const int listIndex = this->m_Actions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		CString scriptId;
		CString buffer;
		//_showCStr(scriptId);
		//_showCStr(buffer);
		this->m_ScriptType.GetLBText(scriptIndex, scriptId);
		TruncSpace(scriptId);
		//_showCStr(scriptId);
		buffer.Format("%d", listIndex);
		// _showCStr(buffer);
		buffer = doc.GetStringOr(scriptId, buffer, "0,0");
		// _showCStr(buffer);
		int commaPos = buffer.Find(',');
		if (commaPos < 0) {
			buffer = "0";
		} else {
			buffer = buffer.Mid(commaPos + 1);
		}

		int actionIndex = this->m_ActionType.GetCurSel();
		if (actionIndex >= 0) {
			const int actionData = this->m_ActionType.GetItemData(actionIndex);
			this->UpdateParams(actionData);
			actionIndex = this->m_Param.FindString(0, buffer);
			if (actionIndex != CB_ERR) {
				this->m_Param.SetCurSel(actionIndex);
			}
			CString listStr;
			CString tmp;
			tmp.Format("%d,%s", actionData, buffer.operator LPCSTR());
			listStr.Format("%d", listIndex);
			doc.SetString(scriptId, listStr, tmp);
			// _showCStr(tmp);
		}
	}
}

void CScriptTypes::OnSelchangeActionType()
{
	int curActionIdx = this->m_ActionType.GetCurSel();
	if (curActionIdx >= 0) {
		int curActionData = this->m_ActionType.GetItemData(curActionIdx);
		auto const& dict = m_actionDefinitions;
		auto itr = dict.find(curActionData);
		if (itr != dict.end()) {
			this->OnEditchangeActionType();

			this->m_Description.SetWindowTextA(itr->second.Description_);
			this->m_Description.EnableWindow(itr->second.Editable_);
			this->m_Param.EnableWindow(itr->second.Editable_);
		}
	}
}

int CScriptTypes::getExtraValue()
{
	auto const curActionSel = this->m_ActionType.GetCurSel();
	const int actionData = this->m_ActionType.GetItemData(curActionSel);
	auto const paramType = getParameterType(actionData);
	auto const extParamType = getExtraParamType(paramType);
	CString curExtParamContent;
	m_ParamExt.GetWindowText(curExtParamContent);
	//errstream << " curExtParamContent = " << curExtParamContent;
	return atoi(curExtParamContent);
}

void CScriptTypes::OnEditchangeParam()
{
	auto& doc = Map->GetIniFile();
	CString scriptId, buffer, listStr, paramStr, tmp;
	int scriptIndex, listIndex, actionIndex;

	scriptIndex = this->m_ScriptType.GetCurSel();
	listIndex = this->m_Actions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->m_ScriptType.GetLBText(scriptIndex, scriptId);
		TruncSpace(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetStringOr(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR) {
			actionIndex = buffer.GetLength();
		}
		buffer = buffer.Mid(0, actionIndex);
		this->m_Param.GetWindowText(paramStr);
		//paramStr = this->m_Param.GetText();
		TruncSpace(paramStr);
		//
		//LogDebug(" actionData = %d, paramType = %d, extParamType = %d", actionData, paramType, extParamType);
		if (auto const extValue = getExtraValue()) {
			auto const paramInt = MAKEWPARAM(atoi(paramStr), extValue);
			paramStr.Format("%d", paramInt);
		}
		//
		tmp.Format("%s,%s", buffer, paramStr);
		listStr.Format("%d", listIndex);
		doc.SetString(scriptId, listStr, tmp);
		//_showCStr(tmp);
	}
}

void CScriptTypes::OnSelchangeParam()
{
	m_Param.SetWindowText(GetText(&m_Param));
	OnEditchangeParam();
}

void CScriptTypes::OnAddaction()
{
	CIniFile& ini = Map->GetIniFile();
	CString Scripttype;
	if (m_ScriptType.GetCurSel() >= 0) {
		m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
		TruncSpace(Scripttype);
		int count = ini[Scripttype].Size() - 1;
		CString action;
		action.Format("%d", count);
		ini.SetString(Scripttype, action, "0,0");
		UpdateDialog();
	}
}

void CScriptTypes::OnDeleteaction()
{
	CIniFile& ini = Map->GetIniFile();

	CString Scripttype;
	if (m_Actions.GetCurSel() < 0) {
		return;
	}
	if (m_ScriptType.GetCurSel() < 0) {
		return;
	}
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);


	// okay, action is now the deleted one...
	int i;
	for (i = m_Actions.GetCurSel(); i < m_Actions.GetCount() - 1; i++) {
		// okay, now move every action one number up.
		char current[50];
		char next[50];

		itoa(i, current, 10);
		itoa(i + 1, next, 10);

		ini.SetString(Scripttype, current, ini.GetString(Scripttype, next));
	}
	char last[50];
	itoa(m_Actions.GetCount() - 1, last, 10);
	ini.RemoveValueByKey(Scripttype, last);

	UpdateDialog();
}

CString GetFree(const char* section);

void CScriptTypes::insertScriptType(const CString& name, const std::vector<CString>& items)
{
	CString ID = GetFreeID();
	CString p = GetFree("ScriptTypes");

	CIniFile& ini = Map->GetIniFile();
	auto& sec = ini.AddSection(ID);
	ini.SetString("ScriptTypes", p, ID);
	sec.SetString("Name", name);

	// template value copy
	auto idx = 0;
	CString idStr;
	for (auto const& line : items) {
		idStr.Format("%d", idx++);
		sec.SetString(idStr, line);
	}
	((CFinalSunDlg*)theApp.m_pMainWnd)->UpdateDialogs(TRUE);
	for (auto i = 0; i < m_ScriptType.GetCount(); i++) {
		CString data;
		m_ScriptType.GetLBText(i, data);
		TruncSpace(data);

		if (data == ID) {
			m_ScriptType.SetCurSel(i);
			OnSelchangeScripttype(); // MW bugfix
			break;
		}
	}
}

void CScriptTypes::OnAdd()
{
	errstream << "Add Script" << std::endl;
	int curTemplateIndex = m_Template.GetCurSel();
	auto const& curTemplate = m_scriptTemplates[curTemplateIndex];
	errstream << "Now using Script Template: " << curTemplate.Name() << std::endl;
	insertScriptType(curTemplate.Name(), curTemplate.Content());
}

void CScriptTypes::OnDelete()
{
	CIniFile& ini = Map->GetIniFile();

	CString Scripttype;
	if (m_ScriptType.GetCurSel() < 0) {
		return;
	}
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);

	int res = MessageBox("Are you sure to delete this ScriptType? Don´t forget to delete any references to this ScriptType", "Delete ScriptType", MB_YESNO | MB_ICONQUESTION);
	if (res != IDYES) {
		return;
	}

	ini.DeleteSection(Scripttype);
	auto const& id = ini["ScriptTypes"].Nth(ini["ScriptTypes"].FindValue(Scripttype)).first;
	ini.RemoveValueByKey("ScriptTypes", id);
	//UpdateDialog();
	((CFinalSunDlg*)theApp.m_pMainWnd)->UpdateDialogs(TRUE);
}



void CScriptTypes::ListBehaviours(CComboBox& cb)
{
	while (cb.DeleteString(0) != CB_ERR);

	cb.AddString(TranslateStringACP("0 - Sleep"));
	cb.AddString(TranslateStringACP("1 - Attack nearest enemy"));
	cb.AddString(TranslateStringACP("2 - Move"));
	cb.AddString(TranslateStringACP("3 - QMove"));
	cb.AddString(TranslateStringACP("4 - Retreat home for R&R"));
	cb.AddString(TranslateStringACP("5 - Guard"));
	cb.AddString(TranslateStringACP("6 - Sticky (never recruit)"));
	cb.AddString(TranslateStringACP("7 - Enter object"));
	cb.AddString(TranslateStringACP("8 - Capture object"));
	cb.AddString(TranslateStringACP("9 - Move into & get eaten"));
	cb.AddString(TranslateStringACP("10 - Harvest"));
	cb.AddString(TranslateStringACP("11 - Area Guard"));
	cb.AddString(TranslateStringACP("12 - Return (to refinery)"));
	cb.AddString(TranslateStringACP("13 - Stop"));
	cb.AddString(TranslateStringACP("14 - Ambush (wait until discovered)"));
	cb.AddString(TranslateStringACP("15 - Hunt"));
	cb.AddString(TranslateStringACP("16 - Unload"));
	cb.AddString(TranslateStringACP("17 - Sabotage (move in & destroy)"));
	cb.AddString(TranslateStringACP("18 - Construction"));
	cb.AddString(TranslateStringACP("19 - Deconstruction"));
	cb.AddString(TranslateStringACP("20 - Repair"));
	cb.AddString(TranslateStringACP("21 - Rescue"));
	cb.AddString(TranslateStringACP("22 - Missile"));
	cb.AddString(TranslateStringACP("23 - Harmless"));
	cb.AddString(TranslateStringACP("24 - Open"));
	cb.AddString(TranslateStringACP("25 - Patrol"));
	cb.AddString(TranslateStringACP("26 - Paradrop approach drop zone"));
	cb.AddString(TranslateStringACP("27 - Paradrop overlay drop zone"));
	cb.AddString(TranslateStringACP("28 - Wait"));
	cb.AddString(TranslateStringACP("29 - Attack again"));
	if (yuri_mode) {
		cb.AddString(TranslateStringACP("30 - Spyplane approach"));
		cb.AddString(TranslateStringACP("31 - Spyplane retreat"));
	}
}

BOOL CScriptTypes::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateStrings();

	while (m_ActionType.DeleteString(0) != CB_ERR);


	char* pParseBuffer[2];
	for (auto& [idStr, content] : g_data["ScriptParams"]) {
		int id = atoi(idStr);
		if (id < 0) {
			continue;
		}
		auto const param1 = GetParam(content, 0);
		// no valid value at all
		if (param1.IsEmpty()) {
			continue;
		}
		m_paramDefinitions[id].Label_ = param1;
		auto const param2 = GetParam(content, 1);
		if (!param2.IsEmpty()) {
			m_paramDefinitions[id].Type_ = ParameterType(atoi(param2));
		}
	}

	for (auto& pair : g_data["ScriptsRA2"]) {
		int id = atoi(pair.first);
		//LogDebug(" id = %d", id);
		if (id < 0) {
			continue;
		}
		auto const& strings = INIHelper::Split(pair.second);
		//LogDebug("pair.second = %s, count = %d", pair.second, strings.size());
		switch (strings.size()) {
			case 5:
				m_actionDefinitions[id].Description_ = strings[4];
				//LogDebug(" Description_ = %s", m_actionDefinitions[id].Description_);
			case 4:
				m_actionDefinitions[id].Editable_ = INIHelper::StringToBool(strings[3], false);
			case 3:
				m_actionDefinitions[id].Hide_ = INIHelper::StringToBool(strings[2], false);
			case 2:
				m_actionDefinitions[id].ParamTypeIndex_ = atoi(strings[1]);
			case 1:
				m_actionDefinitions[id].Name_ = strings[0];
			case 0:
			default:
				continue;
		}
	}

	int counter = 0;
	for (auto& ent : m_actionDefinitions) {
		if (!ent.second.Hide_) {
			int data = m_ActionType.AddString(ent.second.Name_);
			m_ActionType.SetItemData(data, counter);
		}
		++counter;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CScriptTypes::OnCbnSelchangeScriptExtra()
{
	// TODO: Add your control notification handler code here
}


void CScriptTypes::OnCbnSelchangeScriptTemplate()
{

}


void CScriptTypes::OnBnClickedScriptCopy()
{
	auto& doc = Map->GetIniFile();

	int nCurSel = this->m_ScriptType.GetCurSel();
	if (nCurSel < 0) {
		return;
	}
	CString label;
	this->m_ScriptType.GetLBText(nCurSel, label);
	TruncSpace(label);

	auto const& sec = doc[label];
	auto const name = sec.GetString("Name") + " Clone";
	std::vector<CString> contents;

	// don't try infinitely
	CString idxStr;
	for (auto idx = 0; idx < std::numeric_limits<short>::max(); ++idx) {
		idxStr.Format("%d", idx);
		auto const& item = sec.GetString(idxStr);
		if (!item.IsEmpty()) {
			contents.emplace_back(item);
			continue;
		}
		break;
	}
	this->insertScriptType(name, contents);
}


void CScriptTypes::updateExtraParamComboBox(ExtraParameterType type, int value)
{
	//LogDebug(" type = %d", type);
	HWND text = ::GetDlgItem(this->m_hWnd, IDC_SCRIPT_EXDESC);
	switch (type) {
		default:
		case ExtraParameterType::None:
			::EnableWindow(text, FALSE);
			ComboBoxHelper::Clear(m_ParamExt);
			m_ParamExt.EnableWindow(false);
			m_ParamExt.SetWindowText("");
			break;
		case ExtraParameterType::ScanType:
		{
			::EnableWindow(text, TRUE);
			ComboBoxHelper::Clear(m_ParamExt);
			m_ParamExt.EnableWindow(true);
			m_ParamExt.AddString(TranslateStringACP("0 - Least threat"));
			m_ParamExt.AddString(TranslateStringACP("1 - Most threat"));
			m_ParamExt.AddString(TranslateStringACP("2 - Least distant"));
			m_ParamExt.AddString(TranslateStringACP("3 - Most distant"));
			m_ParamExt.SetCurSel(value);
			char buffer[0x20];
			_itoa_s(value, buffer, 10);
			m_ParamExt.SetWindowTextA(buffer);
			//LogDebug(" [%X] window enabled", m_ParamExt.GetHWND());
		}
		break;
		case ExtraParameterType::Counter:
		{
			::EnableWindow(text, TRUE);
			ComboBoxHelper::Clear(m_ParamExt);
			m_ParamExt.EnableWindow(true);
			char buffer[0x20];
			_itoa_s(value, buffer, 10);
			m_ParamExt.SetWindowTextA(buffer);
			break;
		}
	}
}

const CScriptTypes::CScriptTypeAction& CScriptTypes::getActionData(int actionCbIndex) const
{
	return m_actionDefinitions.at(actionCbIndex);
}

const CScriptTypes::CScriptTypeParam& CScriptTypes::getParamData(int paramIndex) const
{
	return m_paramDefinitions.at(paramIndex);
}
ParameterType CScriptTypes::getParameterType(int actionCbIndex) const
{
	return getParamData(getActionData(actionCbIndex).ParamTypeIndex_).Type_;
}

void CScriptTypes::updateExtraValue(ParameterType paramType, CString* paramNumStr)
{
	int extraValue = 0;
	if (paramType == PRM_BuildingType) {
		if (paramNumStr) {
			DWORD rawNum = atoi(*paramNumStr);
			paramNumStr->Format("%d", LOWORD(rawNum));
			extraValue = HIWORD(rawNum);
		}
	}
	errstream << "paramType " << paramType << std::endl;
	updateExtraParamComboBox(getExtraParamType(paramType), extraValue);
}

static void ListSplitGroup(CComboBox& comboBox)
{
	while (comboBox.DeleteString(0) != -1);

	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("0 - Keep Transports, Keep Units")), 0);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("1 - Keep Transports, Lose Units")), 1);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("2 - Lose Transports, Keep Units")), 2);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("3 - Lose Transports, Lose Units")), 3);
}

static void ListScriptLine(const CScriptTypes::ActionDefinitionMap& actionDef, CComboBox& comboBox, CComboBox& currentScript, CListBox& listBox)
{
	int itemNum = listBox.GetCount();
	// up to 50
	if (itemNum > 50) {
		itemNum = 50;
	}

	while (comboBox.DeleteString(0) != -1);

	auto const& doc = Map->GetIniFile();

	CString buffer, scriptName, parambuf;
	currentScript.GetLBText(currentScript.GetCurSel(), scriptName);
	TruncSpace(scriptName);

	for (int i = 0; i < itemNum; ++i) {
		buffer.Format("%d", i);
		buffer = doc.GetStringOr(scriptName, buffer, "0,0");
		int actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR) {
			actionIndex = -1;
		} else {
			actionIndex = atoi(buffer.Mid(0, actionIndex));
		}
		buffer.Format("%d - %s", i + 1, actionDef.at(actionIndex).Name_);
		int idx = comboBox.AddString(buffer);
		comboBox.SetItemData(idx, i);
	}
}

static void ListTypeList(CComboBox& comboBox, const CString& listID)
{
	while (comboBox.DeleteString(0) != -1);

	auto const& doc = Map->GetIniFile();

	for (auto const& [idxStr, id] : doc[listID]) {
		CString text;
		if (doc.TryGetSection(id)) {
			int idx = atoi(idxStr);
			text = doc.GetString(id, "Name");
			text.Format("%d - %s - %s", idx, id, text);
			comboBox.SetItemData(comboBox.AddString(text), idx);
		}
	}
}

static void listScriptTypes(CComboBox& comboBox)
{
	ListTypeList(comboBox, "ScriptTypes");
}

static void listTeamTypes(CComboBox& comboBox)
{
	ListTypeList(comboBox, "TeamTypes");
}

static void listFacing(CComboBox& comboBox)
{
	while (comboBox.DeleteString(0) != -1);

	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("0 - NE")), 0);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("1 - E")), 1);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("2 - SE")), 2);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("3 - S")), 3);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("4 - SW")), 4);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("5 - W")), 5);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("6 - NW")), 6);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("7 - N")), 7);
}

static void listTalkBubble(CComboBox& comboBox)
{
	while (comboBox.DeleteString(0) != -1);

	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("0 - None")), 0);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("1 - Asterisk(*)")), 1);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("2 - Question mark(?)")), 2);
	comboBox.SetItemData(comboBox.AddString(TranslateStringACP("3 - Exclamation mark(!)")), 3);
}

void CScriptTypes::UpdateParams(int actionIndex, CString* paramNumStr)
{
	static int LastActionID = -1;
	auto const& actionDefinition = getActionData(actionIndex);
	auto const& paramDefinition = getParamData(actionDefinition.ParamTypeIndex_);
	auto const paramType = paramDefinition.Type_;
	auto const lastActionID = std::exchange(LastActionID, actionIndex);

	//LogDebug(
	//	" LastActionID = " + std::to_string(lastActionID) +
	//	" actionIndex = " + std::to_string(actionIndex) +
	//	" paramType = " + std::to_string(paramType)
	//);
	if (lastActionID == actionIndex) {
		return;
	}
	updateExtraValue(paramType, paramNumStr);
	switch (paramType) {
		default:
		case PRM_None:
			while (this->m_Param.DeleteString(0) != -1);
			break;
		case 1:
			ListTargets(this->m_Param);
			break;
		case 2:
			ListWaypoints(this->m_Param);
			break;
		case 3:
			ListScriptLine(
				m_actionDefinitions,
				this->m_Param,
				this->m_ScriptType,
				this->m_Actions
			);
			break;
		case 4:
			ListSplitGroup(this->m_Param);
			break;
		case 5:
			ListRulesGlobals(this->m_Param);
			break;
		case 6:
			listScriptTypes(this->m_Param);
			break;
		case 7:
			listTeamTypes(this->m_Param);
			break;
		case 8:
			ListHouses(this->m_Param);
			break;
		case 9:
			ListSpeeches(this->m_Param);
			break;
		case 10:
			ListSounds(this->m_Param);
			break;
		case 11:
			ListMovies(this->m_Param, true);
			break;
		case 12:
			ListThemes(this->m_Param);
			break;
		case 13:
			ComboBoxHelper::ListCountries(this->m_Param);
			break;
		case 14:
			ListMapVariables(this->m_Param);
			break;
		case 15:
			listFacing(this->m_Param);
			break;
		case PRM_BuildingType:
			ListBuildings(this->m_Param);
			break;
		case 17:
			ListAnimations(this->m_Param);
			break;
		case 18:
			listTalkBubble(this->m_Param);
			break;
		case 19:
			ListBehaviours(this->m_Param);
			break;
		case 20:
			ComboBoxHelper::ListBoolean(this->m_Param);
			break;
	}
	this->m_ParamDesc.SetWindowText(paramDefinition.Label_);
	this->m_ParamDesc.EnableWindow(actionDefinition.Editable_);
	this->m_Param.EnableWindow(actionDefinition.Editable_);
	this->m_Description.SetWindowText(actionDefinition.Description_);
}