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

// Lighting.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FinalSun.h"
#include "Lighting.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString SEC_LIGTNINGS = "Lighting";

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CLighting 

IMPLEMENT_DYNCREATE(CLighting, CDialog)

CLighting::CLighting() : CDialog(CLighting::IDD)
{
	//{{AFX_DATA_INIT(CLighting)
	//}}AFX_DATA_INIT
}

CLighting::~CLighting()
{
}

void CLighting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLighting, CDialog)
	ON_EN_CHANGE(IDC_AMBIENT, OnChangeAmbient)
	ON_EN_CHANGE(IDC_LEVEL, OnChangeLevel)
	ON_EN_CHANGE(IDC_RED, OnChangeRed)
	ON_EN_CHANGE(IDC_GREEN, OnChangeGreen)
	ON_EN_CHANGE(IDC_BLUE, OnChangeBlue)
	ON_EN_CHANGE(IDC_AMBIENT2, OnChangeAmbient2)
	ON_EN_CHANGE(IDC_LEVEL2, OnChangeLevel2)
	ON_EN_CHANGE(IDC_RED2, OnChangeRed2)
	ON_EN_CHANGE(IDC_GREEN2, OnChangeGreen2)
	ON_EN_CHANGE(IDC_BLUE2, OnChangeBlue2)
	ON_EN_CHANGE(IDC_AMBIENT3, OnChangeAmbient3)
	ON_EN_CHANGE(IDC_LEVEL3, OnChangeLevel3)
	ON_EN_CHANGE(IDC_RED3, OnChangeRed3)
	ON_EN_CHANGE(IDC_GREEN3, OnChangeGreen3)
	ON_EN_CHANGE(IDC_BLUE3, OnChangeBlue3)
	ON_EN_CHANGE(IDC_AMBIENTDOMINATOR, OnChangeAmbientDominator)
	ON_EN_CHANGE(IDC_AMBIENTNUKE, OnChangeAmbientNuke)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLighting 

void CLighting::UpdateDialog()
{
	CIniFile& ini = Map->GetIniFile();

	ddxReadFromMap(IDC_AMBIENT, "Ambient");
	ddxReadFromMap(IDC_LEVEL, "Level");
	ddxReadFromMap(IDC_RED, "Red");
	ddxReadFromMap(IDC_GREEN, "Green");
	ddxReadFromMap(IDC_BLUE, "Blue");
	ddxReadFromMap(IDC_GROUND, "Ground");

	ddxReadFromMap(IDC_AMBIENT2, "IonAmbient");
	ddxReadFromMap(IDC_LEVEL2, "IonLevel");
	ddxReadFromMap(IDC_RED2, "IonRed");
	ddxReadFromMap(IDC_GREEN2, "IonGreen");
	ddxReadFromMap(IDC_BLUE2, "IonBlue");
	ddxReadFromMap(IDC_GROUND2, "IonGround");

	ddxReadFromMap(IDC_AMBIENT3, "DominatorAmbient");
	ddxReadFromMap(IDC_LEVEL3, "DominatorLevel");
	ddxReadFromMap(IDC_RED3, "DominatorRed");
	ddxReadFromMap(IDC_GREEN3, "DominatorGreen");
	ddxReadFromMap(IDC_BLUE3, "DominatorBlue");
	ddxReadFromMap(IDC_GROUND3, "DominatorGround");
	ddxReadFromMap(IDC_AMBIENTDOMINATOR, "DominatorAmbientChangeRate");
	ddxReadFromMap(IDC_AMBIENTNUKE, "NukeAmbientChangeRate");
}

void CLighting::translateUI()
{
	TranslateWindowCaption(*this, "Lighting");

	TranslateDlgItem(*this, IDC_TASKFORCE_T_TYPE, "TaskforcesType");
	TranslateDlgItem(*this, IDD_LIGHTING, "Lighting");
	TranslateDlgItem(*this, IDC_DESC, "LightingDesc");
	TranslateDlgItem(*this, IDC_LNORMAL, "LightingNormal");
	TranslateDlgItem(*this, IDC_LDOMINATOR, "LightingDominator");
	// Others
	TranslateDlgItem(*this, IDC_LAMBIENTNUKE, "LightingNukeAmbientChangeRate");
	TranslateDlgItem(*this, IDC_LAMBIENTDOMINATOR, "LightingDominatorAmbientChangeRate");

	TranslateDlgItem(*this, IDC_LAMBIENT1, "LightingAmbient");
	TranslateDlgItem(*this, IDC_LGREEN1, "LightingGreen");
	TranslateDlgItem(*this, IDC_LRED1, "LightingRed");
	TranslateDlgItem(*this, IDC_LBLUE1, "LightingBlue");
	TranslateDlgItem(*this, IDC_LLEVEL1, "LightingLevel");
	TranslateDlgItem(*this, IDC_LGROUND1, "LightingGround");
	// IonStorm
#ifdef RA2_MODE
	TranslateDlgItem(*this, IDC_LIONSTORM, "LightingWeatherStorm");
#else
	TranslateDlgItem(*this, IDC_LIONSTORM, "LightingIonStorm");
#endif
	TranslateDlgItem(*this, IDC_LAMBIENT2, "LightingAmbient");
	TranslateDlgItem(*this, IDC_LGREEN2, "LightingGreen");
	TranslateDlgItem(*this, IDC_LRED2, "LightingRed");
	TranslateDlgItem(*this, IDC_LBLUE2, "LightingBlue");
	TranslateDlgItem(*this, IDC_LLEVEL2, "LightingLevel");
	TranslateDlgItem(*this, IDC_LGROUND2, "LightingGround");

	// Dominator
	TranslateDlgItem(*this, IDC_LAMBIENT3, "LightingAmbient");
	TranslateDlgItem(*this, IDC_LGREEN3, "LightingGreen");
	TranslateDlgItem(*this, IDC_LRED3, "LightingRed");
	TranslateDlgItem(*this, IDC_LBLUE3, "LightingBlue");
	TranslateDlgItem(*this, IDC_LLEVEL3, "LightingLevel");
	TranslateDlgItem(*this, IDC_LGROUND3, "LightingGround");
}

BOOL CLighting::OnInitDialog()
{
	auto const ret = CDialog::OnInitDialog();
	translateUI();
	return ret;
}

void CLighting::ddxWithMap(const int controlID, const CString& key, const DdxMode mode)
{
	::ddxWithMap(*GetDlgItem(controlID), SEC_LIGTNINGS, key, mode);
}
void CLighting::ddxReadFromMap(const int controlID, const CString& key)
{
	ddxWithMap(controlID, key, DDX_ReadFromIni);
}
void CLighting::ddxWriteIntoMap(const int controlID, const CString& key)
{
	ddxWithMap(controlID, key, DDX_WriteToIni);
}
// Normal Lighting
void CLighting::OnChangeAmbient()
{
	ddxWriteIntoMap(IDC_AMBIENT, "Ambient");
}
void CLighting::OnChangeLevel()
{
	ddxWriteIntoMap(IDC_LEVEL, "Level");
}
void CLighting::OnChangeRed()
{
	ddxWriteIntoMap(IDC_RED, "Red");
}
void CLighting::OnChangeGreen()
{
	ddxWriteIntoMap(IDC_GREEN, "Green");
}
void CLighting::OnChangeBlue()
{
	ddxWriteIntoMap(IDC_BLUE, "Blue");
}
// IonStorm Lighting
void CLighting::OnChangeAmbient2()
{
	ddxWriteIntoMap(IDC_AMBIENT2, "IonAmbient");
}
void CLighting::OnChangeLevel2()
{
	ddxWriteIntoMap(IDC_LEVEL2, "IonLevel");
}
void CLighting::OnChangeRed2()
{
	ddxWriteIntoMap(IDC_RED2, "IonRed");
}
void CLighting::OnChangeGreen2()
{
	ddxWriteIntoMap(IDC_GREEN2, "IonGreen");
}
void CLighting::OnChangeBlue2()
{
	ddxWriteIntoMap(IDC_BLUE2, "IonBlue");
}
// Dominator Lighting
void CLighting::OnChangeAmbient3()
{
	ddxWriteIntoMap(IDC_AMBIENT3, "DominatorAmbient");
}
void CLighting::OnChangeLevel3()
{
	ddxWriteIntoMap(IDC_LEVEL3, "DominatorLevel");
}
void CLighting::OnChangeRed3()
{
	ddxWriteIntoMap(IDC_RED3, "DominatorRed");
}
void CLighting::OnChangeGreen3()
{
	ddxWriteIntoMap(IDC_GREEN3, "DominatorGreen");
}
void CLighting::OnChangeBlue3()
{
	ddxWriteIntoMap(IDC_BLUE3, "DominatorBlue");
}
void CLighting::OnChangeAmbientDominator()
{
	ddxWriteIntoMap(IDC_AMBIENTDOMINATOR, "DominatorAmbientChangeRate");
}
void CLighting::OnChangeAmbientNuke()
{
	ddxWriteIntoMap(IDC_AMBIENTNUKE, "NukeAmbientChangeRate");
}
