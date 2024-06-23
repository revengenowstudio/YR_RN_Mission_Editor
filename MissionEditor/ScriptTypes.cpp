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


#define TMISSION_COUNT 59
#define TPROPERTY_COUNT 4
#define UNLOAD_COUNT 4


// The various missions that a team can have.
enum TeamMissionType {
	TMISSION_ATTACK,
	TMISSION_ATT_WAYPT,
	TMISSION_BERZERK,
	TMISSION_MOVE,
	TMISSION_MOVECELL,
	TMISSION_GUARD,
	TMISSION_LOOP,
	TMISSION_WIN,
	TMISSION_UNLOAD,
	TMISSION_DEPLOY,
	TMISSION_HOUND_DOG,
	TMISSION_DO,
	TMISSION_SET_GLOBAL,
	TMISSION_IDLE_ANIM,
	TMISSION_LOAD,
	TMISSION_SPY,
	TMISSION_PATROL,
	TMISSION_SCRIPT,
	TMISSION_TEAMCHANGE,
	TMISSION_PANIC,
	TMISSION_CHANGE_HOUSE,
	TMISSION_SCATTER,
	TMISSION_GOTO_SHROUD,
	TMISSION_LOSE,
	TMISSION_PLAY_SPEECH,
	TMISSION_PLAY_SOUND,
	TMISSION_PLAY_MOVIE,
	TMISSION_PLAY_MUSIC,
	TMISSION_REDUCE_TIBERIUM,
	TMISSION_BEGIN_PRODUCTION,
	TMISSION_FIRE_SALE,
	TMISSION_SELF_DESTRUCT,
	TMISSION_ION_STORM_START,
	TMISSION_ION_STORM_END,
	TMISSION_CENTER_VIEWPOINT,
	TMISSION_RESHROUD,
	TMISSION_REVEAL,
	TMISSION_DESTROY_MEMBERS,
	TMISSION_CLEAR_GLOBAL,
	TMISSION_SET_LOCAL,
	TMISSION_CLEAR_LOCAL,
	TMISSION_UNPANIC,
	TMISSION_FORCE_FACING,
	TMISSION_FULLY_LOADED,
	TMISSION_UNLOAD_TRUCK,
	TMISSION_LOAD_TRUCK,
	TMISSION_ATTACK_BUILDING_WITH_PROPERTY,
	TMISSION_MOVETO_BUILDING_WITH_PROPERTY,
	TMISSION_SCOUT,
	TMISSION_SUCCESS,
	TMISSION_FLASH,
	TMISSION_PLAY_ANIM,
	TMISSION_TALK_BUBBLE,
	TMISSION_GATHER_FAR,
	TMISSION_GATHER_NEAR,
	TMISSION_ACTIVATE_CURTAIN,
	TMISSION_CHRONO_ATTACK_BUILDING_WITH_PROPERTY,
	TMISSION_CHRONO_ATTACK,
	TMISSION_MOVETO_OWN_BUILDING_WITH_PROPERTY,
};

char const* TMissions[TMISSION_COUNT] = {
	"Attack...",
	"Attack Waypoint...",
	"Go Berzerk",
	"Move to waypoint...",
	"Move to Cell...",
	"Guard area (timer ticks)...",
	"Jump to line #...",
	"Player wins",
	"Unload...",
	"Deploy",
	"Follow friendlies",
	"Do this...",
	"Set global...",
	"Idle Anim...",
	"Load onto Transport",
	"Spy on bldg @ waypt...",
	"Patrol to waypoint...",
	"Change script...",
	"Change team...",
	"Panic",
	"Change house...",
	"Scatter",
	"Goto nearby shroud",
	"Player loses",
	"Play speech...",
	"Play sound...",
	"Play movie...",
	"Play music...",
	"Reduce tiberium",
	"Begin production",
	"Fire sale",
	"Self destruct",
	"Ion storm start in...",
	"Ion storn end",
	"Center view on team (speed)...",
	"Reshroud map",
	"Reveal map",
	"Delete team members",
	"Clear global...",
	"Set local...",
	"Clear local...",
	"Unpanic",
	"Force facing...",
	"Wait till fully loaded",
	"Truck unload",
	"Truck load",
	"Attack enemy building",
	"Moveto enemy building",
	"Scout",
	"Success",
	"Flash",
	"Play Anim",
	"Talk Bubble",
	"Gather at Enemy",
	"Gather at Base",
	"Iron Curtain Me",
	"Chrono Prep for ABwP",
	"Chrono Prep for AQ",
	"Move to own building",
};

char const* TMissionsHelp[TMISSION_COUNT] = {
	"Attack some general target",
	"Attack anything nearby the specified waypoint",
	"Cyborg members of the team will go berzerk.",
	"Orders the team to move to a waypoint on the map",
	"Orders the team to move to a specific cell on the map",
	"Guard an area for a specified amount of time",
	"Move to a new line number in the script.  Used for loops.",
	"Duh",
	"Unloads all loaded units.  The command parameter specifies which units should stay a part of the team, and which should be severed from the team.",
	"Causes all deployable units in the team to deploy",
	"Causes the team to follow the nearest friendly unit",
	"Give all team members the specified mission",
	"Sets a global variable",
	"Causes team members to enter their idle animation",
	"Causes all units to load into transports, if able",
	"**OBSOLETE**",
	"Move to a waypoint while scanning for enemies",
	"Causes the team to start using a new script",
	"Causes the team to switch team types",
	"Causes all units in the team to panic",
	"All units in the team switch houses",
	"Tells all units to scatter",
	"Causes units to flee to a shrouded cell",
	"Causes the player to lose",
	"Plays the specified voice file",
	"Plays the specified sound file",
	"Plays the specified movie file",
	"Plays the specified theme",
	"Reduces the amount of tiberium around team members",
	"Signals the owning house to begin production",
	"Causes an AI house to sell all of its buildings and do a Braveheart",
	"Causes all team members to self destruct",
	"Causes an ion storm to begin at the specified time",
	"Causes an ion storm to end",
	"Center view on team (speed)...",
	"Reshrouds the map",
	"Reveals the map",
	"Delete all members from the team",
	"Clears the specified global variable",
	"Sets the specified local variable",
	"Clears the specified local variable",
	"Causes all team members to stop panicking",
	"Forces team members to face a certain direction",
	"Waits until all transports are full",
	"Causes all trucks to unload their crates (ie, change imagery)",
	"Causes all trucks to load crates (ie, change imagery)",
	"Attack a specific type of building with the specified property",
	"Move to a specific type of building with the specified property",
	"The team will scout the bases of the players that have not been scouted",
	"Record a team as having successfully accomplished its mission.  Used for AI trigger weighting.  Put this at the end of every AITrigger script.",
	"Flashes a team for a period of team.",
	"Plays an anim over every unit in the team.",
	"Displays talk bubble over first unit in the team.",
	"Uses AISafeDistance to find a spot close to enemy's base to gather close.",
	"Gathers outside own base perimeter.",
	"Calls (and waits if nearly ready) for House to deliver Iron Curtain to Team.",
	"Teleports team to Building With Property, but needs similar attack order as next mission.",
	"Teleports team to Attack Quarry, but needs similar attack order as next mission.",
	"A BwP move that will only search through buildings owned by this house.",
};


char const* TargetProperties[TPROPERTY_COUNT] = {
	"Least Threat",
	"Greatest Threat",
	"Nearest",
	"Farthest",
};

char const* UnloadTypeNames[UNLOAD_COUNT] = {
	"Keep Transports, Keep Units",
	"Keep Transports, Lose Units",
	"Lose Transports, Keep Units",
	"Lose Transports, Lose Units",
};


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

#pragma region ScriptTemplate
//Class ScriptTemplate
ScriptTemplate::ScriptTemplate() {
	Data.clear();
}

ScriptTemplate::ScriptTemplate(std::vector<std::string> init) {
	int count = atoi(init[2].c_str());
	Data.resize(count + 1);
	Data[0].first = init[0];//UIName
	Data[0].second = init[1];//Name
	for (int i = 1; i <= count; ++i) {
		Data[i].first = init[2 * i + 1];
		Data[i].second = init[2 * i + 2];
	}
}

int ScriptTemplate::Count() {
	return Data.size() - 1;
}

void ScriptTemplate::Resize(int size) {
	Data.resize(size);
	return;
}

std::pair<std::string, std::string>& ScriptTemplate::operator[] (int index) {
	return Data[index];
}
const std::pair<std::string, std::string>& ScriptTemplate::operator[] (int index) const {
	auto non_const_ptr = const_cast<std::remove_const_t<std::remove_pointer_t<decltype(this)>>*>(this);
	return non_const_ptr->operator[](index);
}
#pragma endregion

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
	TranslateDlgItem(*this, IDC_DELETE, "ScriptTypesDelScript");

	TranslateDlgItem(*this, IDC_SCTIPTTYPE_INRO, "ScriptTypesDesc");
	TranslateDlgItem(*this, IDC_SCRIPT_TEMPLATE_DSC, "ScriptTypesSelectedTemplate");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_TYPE, "ScriptTypesSelectedScript");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_NAME, "ScriptTypesName");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_ACTIONS, "ScriptTypesActions");
	TranslateDlgItem(*this, IDC_SCRIPTTYPE_ACTIONTYPE, "ScriptTypesActionType");
	TranslateDlgItem(*this, IDC_PDESC, "ScriptTypesActionParam");
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
	ON_CBN_KILLFOCUS(IDC_SCRIPTTYPE, OnEditchangeScripttype)
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

void CScriptTypes::UpdateDialog()
{
	CIniFile& ini = Map->GetIniFile();

	int sel = m_ScriptType.GetCurSel();


	while (m_ScriptType.DeleteString(0) != CB_ERR);


	// MW 07/24/01: clear dialog
	m_Description.SetWindowText("");
	m_Name = "";
	m_Param.SetWindowText("");
	m_ParamExt.SetWindowText("");
	m_Actions.SetWindowText("");
	m_ActionType.SetCurSel(-1);
	m_Template.SetCurSel(-1);

	UpdateData(FALSE);

	for (auto const& [seq, type] : ini["ScriptTypes"]) {
		CString desc = type;
		desc += " (";
		desc += ini.GetString(type, "Name");
		desc += ")";
		m_ScriptType.AddString(desc);
	}

	m_ScriptType.SetCurSel(0);
	if (sel >= 0) {
		m_ScriptType.SetCurSel(sel);
	}
	OnSelchangeScripttype();


}

void CScriptTypes::OnEditchangeScripttype()
{

}

void CScriptTypes::OnSelchangeScripttype()
{
	CIniFile& ini = Map->GetIniFile();

	int sel = m_Actions.GetCurSel();
	while (m_Actions.DeleteString(0) != CB_ERR);

	CString Scripttype;
	if (m_ScriptType.GetCurSel() < 0) return;
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
	if (sel >= 0) m_Actions.SetCurSel(sel);
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
		auto const idxs = std::to_string(listIndex);
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
		this->UpdateParams(actionIndex, paramNumStr);
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
	CIniFile& ini = Map->GetIniFile();

	while (m_Param.DeleteString(0) != CB_ERR);
	CString Scripttype;
	char action[50];
	if (m_Actions.GetCurSel() < 0) return;
	if (m_ScriptType.GetCurSel() < 0) return;
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);

	//CString type;
	//m_ActionType.GetWindowText(type);
	//TruncSpace(type);
	//MessageBox("beep");
	int type = m_ActionType.GetCurSel();

	int i;
	char tmp[50];

	switch (type) {
	case 0:
		ListTargets(m_Param);
		m_ParamDesc.SetWindowText("Target:");
		break;
	case 39:
	case 40:
		ListMapVariables(m_Param);
		break;

	case 11:
		ListBehaviours(m_Param);
		break;
	case 1:
	case 3:
	case 16:
		ListWaypoints(m_Param);
		m_ParamDesc.SetWindowText("Waypoint:");
		break;
	case 4:
		m_ParamDesc.SetWindowText("Cell:");
		break;
	case 5:
		m_ParamDesc.SetWindowText("Time units to guard:");
		break;
	case 6:
		m_ParamDesc.SetWindowText("Script action #:");
		while (m_Param.DeleteString(0) != CB_ERR);
		for (i = 1; i <= ini[Scripttype].Size() - 1; i++) {
			m_Param.AddString(itoa(i, tmp, 10));
		}
		break;
	case 8:
		m_ParamDesc.SetWindowText("Split groups:");
		while (m_Param.DeleteString(0) != CB_ERR);
		int i;
		for (i = 0; i < UNLOAD_COUNT; i++) {
			CString p;
			char c[50];
			itoa(i, c, 10);
			p = c;
			p += " - ";
			p += UnloadTypeNames[i];

			m_Param.AddString(p);
		}
		break;
	case 9:
	case 14:
	case 37:
		m_ParamDesc.SetWindowText("Use 0:");
		break;
	case 12:
		m_ParamDesc.SetWindowText("Global:");
		break;
	case 20:
		ListHouses(m_Param, TRUE);
		m_ParamDesc.SetWindowText("House:");
		break;
	case 46:
	case 47:
	{
		m_ParamDesc.SetWindowText("Type to move/attack:");
		auto const& bldTypeSec = rules["BuildingTypes"];
		for (i = 0; i < bldTypeSec.Size(); i++) {
			char c[50];
			itoa(i, c, 10);
			CString s = c;

			s += " ";
			//s+=rules.sections[*rules.sections["BuildingTypes"].GetValue(i)].values["Name"];
			s += Map->GetUnitName(bldTypeSec.Nth(i).second);
			m_Param.AddString(s);
		}
		break;
	}

	default:
		m_ParamDesc.SetWindowText("Parameter of action:");
	}

	itoa(m_Actions.GetCurSel(), action, 10);

	char types[50];
	itoa(type, types, 10);
	ini.SetString(Scripttype, action, SetParam(ini.GetString(Scripttype, action), 0, types));
}

void CScriptTypes::OnSelchangeActionType()
{
	CString str;
	if (m_ActionType.GetCurSel() > -1) {
		//m_ActionType.GetLBText(m_ActionType.GetCurSel(), str);
		//m_ActionType.SetWindowText(str);

		m_Description.SetWindowText(TMissionsHelp[m_ActionType.GetCurSel()]);
	}



	OnEditchangeActionType();
}

void CScriptTypes::OnEditchangeParam()
{
	CIniFile& ini = Map->GetIniFile();

	CString Scripttype;
	char action[50];
	if (m_Actions.GetCurSel() < 0) return;
	if (m_ScriptType.GetCurSel() < 0) return;
	m_ScriptType.GetLBText(m_ScriptType.GetCurSel(), Scripttype);
	TruncSpace(Scripttype);

	CString param;
	m_Param.GetWindowText(param);
	TruncSpace(param);

	param = TranslateHouse(param);

	itoa(m_Actions.GetCurSel(), action, 10);
	ini.SetString(Scripttype, action, SetParam(ini.GetString(Scripttype, action), 1, param));
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
	if (m_Actions.GetCurSel() < 0) return;
	if (m_ScriptType.GetCurSel() < 0) return;
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

void CScriptTypes::OnAdd()
{
	errstream << "Add Script" << std::endl;
	HWND ScriptWnd = this->m_hWnd;

	//HWND EditName = ::GetDlgItem(ScriptWnd, WND_Script::EditScriptName);
	//HWND ListBox = ::GetDlgItem(ScriptWnd, WND_Script::ListBoxActions);
	//HWND ComboType = ::GetDlgItem(ScriptWnd, WND_Script::m_ActionType);
	//HWND ComboPara = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxParameter);
	int curTemplateComboCount = m_Template.GetCount();
	if (curTemplateComboCount <= 0) {
		// TODO: load when dialog init
		//HWND BtnLoad = ::GetDlgItem(ScriptWnd, WND_Script::ButtonReload);
		//::SendMessageA(BtnLoad, WM_LBUTTONDOWN, WND_Script::ButtonReload, NULL);
		//::SendMessageA(BtnLoad, WM_LBUTTONUP, WND_Script::ButtonReload, NULL);
	}
	int curTemplateIndex = m_Template.GetCurSel();
	ScriptTemplate& curTemplate = m_scriptTemplates[curTemplateIndex];
	errstream << "Now using Script Template %s" << curTemplate[0].first.c_str() << std::endl;

	int ScriptCount = m_ScriptType.GetCount();
	std::set<CString> ScriptDictionary;

	for (int i = 0; i < ScriptCount; ++i) {
		int strLen = m_ScriptType.GetLBTextLen(i);
		CString scriptID;
		m_ScriptType.GetLBText(i, scriptID);
		ScriptDictionary.emplace(std::move(scriptID));
	}

	//this->OnScriptTypeAdd();
#if 1
	CIniFile& ini = Map->GetIniFile();

	CString ID = GetFreeID();

	CString p = GetFree("ScriptTypes");
	ini.SetString("ScriptTypes", p, ID);
	ini.SetString(ID, "Name", "New script");



	int i;
	for (i = 0; i < m_ScriptType.GetCount(); i++) {
		CString data;
		m_ScriptType.GetLBText(i, data);
		TruncSpace(data);

		if (data == ID) {
			m_ScriptType.SetCurSel(i);
			OnSelchangeScripttype(); // MW bugfix
			break;
		}
	}

	((CFinalSunDlg*)theApp.m_pMainWnd)->UpdateDialogs(TRUE);
#endif

	int newIdx = 0;
	for (; newIdx < ScriptCount; ++newIdx) {
		CString str;
		m_ScriptType.GetLBText(newIdx, str);
		if (ScriptDictionary.find(str) == ScriptDictionary.end()) {
			break;
		}
	}

	//::SendMessageA(m_ScriptType, CB_SETCURSEL, i, NULL);
	this->m_ScriptType.SetCurSel(newIdx);//select the end
	this->SetDlgItemTextA(IDC_NAME, curTemplate[0].second.c_str());
	this->OnChangeName();

	//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::m_ScriptType, CBN_SELCHANGE), (LPARAM)m_ScriptType);
	this->OnSelchangeScripttype();

	for (int idx = 0; idx < curTemplate.Count(); ++idx) {
		this->OnAddaction();
		auto const& templateItem = curTemplate[idx + 1];//first one is reserved for name
		//::SendMessageA(ListBox, LB_SETCURSEL, idx, NULL);
		this->m_Actions.SetCurSel(idx);
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ListBoxActions, LBN_SELCHANGE), (LPARAM)ListBox);
		this->OnSelchangeActionList();
		//::SendMessageA(ComboType, CB_SETCURSEL, atoi(templateItem->first.c_str()), NULL);
		auto const scripIndex = atoi(templateItem.first.c_str());
		errstream << " m_ActionType cur idx %d: " << atoi(templateItem.first.c_str()) << std::endl;
		this->m_ActionType.SetCurSel(scriptTypeIndexToComboBoxIndex(this->m_ActionType, scripIndex));
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::m_ActionType, CBN_SELCHANGE), (LPARAM)ComboType);
		this->OnSelchangeActionType();
		if (templateItem.second == "EMPTY") {
			continue;
		}
		//::SetWindowTextA(ComboPara, templateItem->second.c_str());
		this->m_Param.SetWindowTextA(templateItem.second.c_str());
		this->OnEditchangeParam();
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ComboBoxParameter, CBN_SELCHANGE), (LPARAM)ComboPara);
	}
}

void CScriptTypes::OnDelete()
{
	CIniFile& ini = Map->GetIniFile();

	CString Scripttype;
	if (m_ScriptType.GetCurSel() < 0) return;
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


	int i;
	for (i = 0; i < TMISSION_COUNT; i++) {
		CString p;
		char c[50];
		itoa(i, c, 10);

		//p=c;


		//p+=" - ";
		p += TMissions[i];

		if (strlen(TMissions[i]) > 0) {
			m_ActionType.AddString(p);
		}
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
	// TODO: Add your control notification handler code here
}


void CScriptTypes::OnBnClickedScriptCopy()
{
	// TODO: Add your control notification handler code here
}


void CScriptTypes::updateExtraParamComboBox(ExtraParameterType type, int value)
{
	//LogDebug(" type = %d", type);
	HWND text = ::GetDlgItem(this->m_hWnd, IDC_SCRIPT_EXDESC);
	switch (type) {
	default:
	case ExtraParameterType::None:
		::EnableWindow(text, FALSE);
		m_ParamExt.Clear();
		m_ParamExt.EnableWindow(false);
		m_ParamExt.SetWindowText("");
		break;
	case ExtraParameterType::ScanType:
	{
		::EnableWindow(text, TRUE);
		m_ParamExt.Clear();
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
		m_ParamExt.Clear();
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

ExtraParameterType getExtraParamType(ParameterType paramType)
{
	switch (paramType) {
	default:
		return ExtraParameterType::None;
	case PRM_BuildingType:
		return ExtraParameterType::ScanType;
	}
}

void CScriptTypes::updateExtraValue(ParameterType paramType, CString& paramNumStr)
{
	int extraValue = 0;
	if (paramType == PRM_BuildingType) {
		DWORD rawNum = atoi(paramNumStr);
		paramNumStr.Format("%d", LOWORD(rawNum));
		extraValue = HIWORD(rawNum);
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

void CScriptTypes::UpdateParams(int actionIndex, CString& paramNumStr)
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
	updateExtraValue(paramType, paramNumStr);
	if (lastActionID == actionIndex) {
		return;
	}
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