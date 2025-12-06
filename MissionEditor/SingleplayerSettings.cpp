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

// SingleplayerSettings.cpp: implementation file
//

#include "stdafx.h"
#include "finalsun.h"
#include "SingleplayerSettings.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// property page CSingleplayerSettings 

IMPLEMENT_DYNCREATE(CSingleplayerSettings, CDialog)

CSingleplayerSettings::CSingleplayerSettings() : CDialog(IDD)
{
	//{{AFX_DATA_INIT(CSingleplayerSettings)

	//}}AFX_DATA_INIT
}

CSingleplayerSettings::~CSingleplayerSettings()
{
}

BOOL CSingleplayerSettings::OnInitDialog()
{
	if (!CDialog::OnInitDialog()) {
		return FALSE;
	}

	translateUI();

	return true;
}

void CSingleplayerSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSingleplayerSettings)
	DDX_Control(pDX, IDC_TIMERINHERIT, m_TimerInherit);
	DDX_Control(pDX, IDC_STARTINGDROPSHIPS, m_StartingDropships);
	DDX_Control(pDX, IDC_FILLSILOS, m_FillSilos);
	DDX_Control(pDX, IDC_CARRYOVERMONEY, m_CarryOverMoney);
	DDX_Control(pDX, IDC_WIN, m_Win);
	DDX_Control(pDX, IDC_PREMAPSELECT, m_PreMapSelect);
	DDX_Control(pDX, IDC_POSTSCORE, m_PostScore);
	DDX_Control(pDX, IDC_LOSE, m_Lose);
	DDX_Control(pDX, IDC_INTRO, m_Intro);
	DDX_Control(pDX, IDC_BRIEF, m_Brief);
	DDX_Control(pDX, IDC_ACTION, m_Action);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSingleplayerSettings, CDialog)
	//{{AFX_MSG_MAP(CSingleplayerSettings)
	ON_CBN_EDITCHANGE(IDC_INTRO, OnEditchangeIntro)
	ON_CBN_EDITCHANGE(IDC_BRIEF, OnEditchangeBrief)
	ON_CBN_EDITCHANGE(IDC_WIN, OnEditchangeWin)
	ON_CBN_EDITCHANGE(IDC_LOSE, OnEditchangeLose)
	ON_CBN_EDITCHANGE(IDC_ACTION, OnEditchangeAction)
	ON_CBN_EDITCHANGE(IDC_POSTSCORE, OnEditchangePostscore)
	ON_CBN_EDITCHANGE(IDC_PREMAPSELECT, OnEditchangePremapselect)
	ON_CBN_EDITCHANGE(IDC_STARTINGDROPSHIPS, OnEditchangeStartingdropships)
	ON_EN_CHANGE(IDC_CARRYOVERMONEY, OnChangeCarryovermoney)
	ON_CBN_EDITCHANGE(IDC_TIMERINHERIT, OnEditchangeTimerinherit)
	ON_CBN_EDITCHANGE(IDC_FILLSILOS, OnEditchangeFillsilos)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handlers

void CSingleplayerSettings::UpdateDialog()
{
	CIniFile& ini = Map->GetIniFile();

	auto const& sec = ini["Basic"];

	auto setIfExists = [&sec](CWnd& wnd, const CString& key) {
		if (auto val = sec.TryGetString(key)) {
			wnd.SetWindowText(*val);
		}
	};

	setIfExists(m_Intro, "Intro");
	setIfExists(m_Brief, "Brief");
	setIfExists(m_Win, "Win");
	setIfExists(m_Lose, "Lose");
	setIfExists(m_Action, "Action");
	setIfExists(m_PostScore, "PostScore");
	setIfExists(m_PreMapSelect, "PreMapSelect");

	setIfExists(m_StartingDropships, "StartingDropships");
	setIfExists(m_CarryOverMoney, "CarryOverMoney");
	setIfExists(m_TimerInherit, "TimerInherit");
	setIfExists(m_FillSilos, "FillSilos");

	ListMovies(m_Intro, TRUE);
	ListMovies(m_Brief, TRUE);
	ListMovies(m_Win, TRUE);
	ListMovies(m_Lose, TRUE);
	ListMovies(m_Action, TRUE);
	ListMovies(m_PostScore, TRUE);
	ListMovies(m_PreMapSelect, TRUE);
}

void CSingleplayerSettings::OnEditchangeIntro()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "Intro", GetText(&m_Intro));
}

void CSingleplayerSettings::OnEditchangeBrief()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "Brief", GetText(&m_Brief));
}

void CSingleplayerSettings::OnEditchangeWin()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "Win", GetText(&m_Win));
}

void CSingleplayerSettings::OnEditchangeLose()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "Lose", GetText(&m_Lose));
}

void CSingleplayerSettings::OnEditchangeAction()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "Action", GetText(&m_Action));
}

void CSingleplayerSettings::OnEditchangePostscore()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "PostScore", GetText(&m_PostScore));
}

void CSingleplayerSettings::OnEditchangePremapselect()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "PreMapSelect", GetText(&m_PreMapSelect));
}

void CSingleplayerSettings::OnEditchangeStartingdropships()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "StartingDropships", GetText(&m_StartingDropships));
}

void CSingleplayerSettings::OnChangeCarryovermoney()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "CarryOverMoney", GetText(&m_CarryOverMoney));
}

void CSingleplayerSettings::OnEditchangeTimerinherit()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "TimerInherit", GetText(&m_TimerInherit));
}

void CSingleplayerSettings::OnEditchangeFillsilos()
{
	CIniFile& ini = Map->GetIniFile();
	ini.SetString("Basic", "FillSilos", GetText(&m_FillSilos));
}

void CSingleplayerSettings::translateUI()
{
	TranslateDlgItem(*this, IDC_LSTARTINGDROPSHIPS, "SingleplayerStartingDropships");
	TranslateDlgItem(*this, IDC_LCARRYOVERMONEY, "SingleplayerCarryOverMoney");
	TranslateDlgItem(*this, IDC_LINHERITTIMER, "SingleplayerTimerInherit");
	TranslateDlgItem(*this, IDC_LFILLSILOS, "SingleplayerFillSilos");
	TranslateDlgItem(*this, IDC_LMOVIES, "SingleplayerMovies");
	TranslateDlgItem(*this, IDC_LINTRO, "SingleplayerIntro");
	TranslateDlgItem(*this, IDC_LBRIEF, "SingleplayerBrief");
	TranslateDlgItem(*this, IDC_LWIN, "SingleplayerWin");
	TranslateDlgItem(*this, IDC_LLOSE, "SingleplayerLose");
	TranslateDlgItem(*this, IDC_LACTION, "SingleplayerAction");
	TranslateDlgItem(*this, IDC_LPOSTSCORE, "SingleplayerPostScore");
	TranslateDlgItem(*this, IDC_LPREMAPSELECT, "SingleplayerPreMapSelect");
	TranslateDlgItem(*this, IDC_DESC, "SingleplayerDesc");

	TranslateWindowCaption(*this, "Singleplayer settings");

	TranslateDlgItem(*this, IDC_LPARTIMEEASY, "SingleplayerParTimeEasy");
	TranslateDlgItem(*this, IDC_LPARTIMEMEDIUM, "SingleplayerParTimeMedium");
	TranslateDlgItem(*this, IDC_LPARTIMEHARD, "SingleplayerParTimeHard");
	TranslateDlgItem(*this, IDC_LOVERPARTITLE, "SingleplayerOverParTitle");
	TranslateDlgItem(*this, IDC_LOVERPARMESSAGE, "SingleplayerOverParMessage");
	TranslateDlgItem(*this, IDC_LUNDERPARTITLE, "SingleplayerUnderParTitle");
	TranslateDlgItem(*this, IDC_LUNDERPARMESSAGE, "SingleplayerUnderParMessage");
	TranslateDlgItem(*this, IDC_RANKINGGROUP, "SingleplayerRanking");
	TranslateDlgItem(*this, IDC_GENERALGROUP, "SingleplayerGeneral");
	TranslateDlgItem(*this, IDC_CAMPAIGNMONEYDELTAEASY, "SingleplayerCampaignMoneyDeltaEasy");
	TranslateDlgItem(*this, IDC_CAMPAIGNMONEYDELTAHARD, "SingleplayerCampaignMoneyDeltaHard");
	TranslateDlgItem(*this, IDC_SPYMONEYSTEALPERCENT, "SingleplayerSpyMoneyStealPercent");
	TranslateDlgItem(*this, IDC_TEAMDELAYS, "SingleplayerTeamDelays");
	TranslateDlgItem(*this, IDC_PRISMSUPPORTMODIFIER, "SingleplayerPrismSupportModifier");
	TranslateDlgItem(*this, IDC_DEFAULTMIRAGEDISGUISES, "SingleplayerDefaultMirageDisguises");
	TranslateDlgItem(*this, IDC_SAVE, "SingleplayerSave");

#ifdef RA2_MODE
	GetDlgItem(IDC_LSTARTINGDROPSHIPS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STARTINGDROPSHIPS)->ShowWindow(SW_HIDE);
#endif
}

void CSingleplayerSettings::PostNcDestroy()
{
	// do not call CDialog::PostNcDestroy();	
	// CDialog::PostNcDestroy();
}

void CSingleplayerSettings::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
}
