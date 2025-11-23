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

// TerrainDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "finalsun.h"
#include "TerrainDlg.h"
#include "TileSetBrowserFrame.h"
#include "mapdata.h"
#include "variables.h"
#include "functions.h"
#include "inlines.h"
#include "GlobalObjectPool.h"
#include "TerrainGroupMgr.h"
#include <string>

extern ACTIONDATA AD;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CTerrainDlg 

CTerrainDlg::CTerrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{

}


void CTerrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerrainDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerrainDlg, CDialogBar)
	//{{AFX_MSG_MAP(CTerrainDlg)
	ON_CBN_SELCHANGE(IDC_TILESET, OnSelchangeTileset)
	ON_CBN_SELCHANGE(IDC_OVERLAY, OnSelchangeOverlay)
	ON_CBN_SELCHANGE(IDC_TERRAINBAR_TGROUP, OnSelchangeTileSetGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTerrainDlg 

void CTerrainDlg::PostNcDestroy()
{
	//delete this;
	//CDialog::PostNcDestroy();
}

void CTerrainDlg::TranslateUI()
{
	TranslateDlgItem(*this, IDD_TERRAINBAR_TG, "TerrainDlgTerrainGround");
	TranslateDlgItem(*this, IDD_TERRAINBAR_OS, "TerrainDlgOverlaySpecial");
	TranslateDlgItem(*this, IDC_TERRAINBAR_MANAGER, "TerrainDlgManagement");
	TranslateDlgItem(*this, IDC_TERRAINBAR_GENERATOR, "TerrainDlgGenerator");
	TranslateDlgItem(*this, IDC_TERRAINBAR_TXT_TGROUP, "TerrainDlgTGroup");
	
}

void CTerrainDlg::OnSelchangeTileset()
{
	CString currentTileSet;
	CComboBox* TileSet;
	TileSet = (CComboBox*)GetDlgItem(IDC_TILESET);
	TileSet->GetLBText(TileSet->GetCurSel(), currentTileSet);

	TruncSpace(currentTileSet);

	auto const frame = reinterpret_cast<CTileSetBrowserFrame*>(GetParentFrame());
	frame->m_view.SetTileSet(atoi(currentTileSet));
	frame->RecalcLayout();
}



BOOL CTerrainDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CTerrainDlg::handleTiles()
{
	if (!tiles) {
		return;
	}
	auto TileSet = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_TILESET));
	while (TileSet->DeleteString(0) != CB_ERR);

	int tilecount = 0;
	CString setId;
	CString setIdxStr;

	auto const cbTerrainGroup = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_TERRAINBAR_TGROUP));
	auto const groupSelected = cbTerrainGroup->GetCurSel();

	for (auto i = 0; i < 10000; i++) {
		setId.Format("%04d", i);
		CString sec = "TileSet" + setId;

		auto const pSec = tiles->TryGetSection(sec);
		if (!pSec) { // no more tileSet type found
			break;
		}
		if (pSec->GetInteger("TilesInSet") == 0) {
			continue;
		}

		auto const& groups = CTerrainGroupManager::Groups();
		// filter in effect
		if (!groups.empty() && groupSelected >= 0) {
			int64_t const groupId = cbTerrainGroup->GetItemData(groupSelected);
			auto const allowed = groupId < 0 
				|| groups.at(groupId).Contains(setId);
			// filtered out
			if (!allowed) {
				continue;
			}
		}

		CString displayStr;
		displayStr.Format("%s (%s)", setId, TranslateStringACP(pSec->GetString("SetName")));

		// force yes
		auto const& theaterType = Map->GetTheater();
		setIdxStr.Format("%d", i);

		auto const bForced = g_data["UseSet" + theaterType].HasValue(setIdxStr);
		auto const bIgnore = g_data["IgnoreSet" + theaterType].HasValue(setIdxStr);

		auto legal = false;
		do {
			if (bForced) {
				legal = true;
				break;
			}
			if (bIgnore) {
				break;
			}
			auto const& tile = (*tiledata)[tilecount];
			if (tile.bMarbleMadness) {
				break;
			}
			if (tile.bAllowToPlace) {
				legal = true;
			}
		} while (0);

		if (legal) {
			TileSet->SetItemData(TileSet->AddString(displayStr), i);
		}

		tilecount += tiles->GetInteger(sec, "TilesInSet");
	}

	TileSet->SetCurSel(0);
	OnSelchangeTileset();
}

void CTerrainDlg::Update()
{
	handleTiles();
	handleTileGroups();

	CComboBox* Overlays;
	Overlays = (CComboBox*)GetDlgItem(IDC_OVERLAY);

	while (Overlays->DeleteString(0) != CB_ERR);


	int e = 0;
	auto const& overlayCache = GlobalObjectPool::Instance().Overlays();
	auto const& types = rules["OverlayTypes"];
	for (auto i = 0; i < types.Size(); i++) {
		CString id = types.Nth(i).second;
		id.Trim();

		if (id.GetLength() > 0) {

			if (rules[id].Exists("Name")) {
				int p;
				BOOL bListIt = TRUE;
				for (p = 0; p < max_ovrl_img; p++) {
					if (auto const pData = overlayCache.Read(i, p); pData && pData->pic) {
						bListIt = TRUE;
					}
				}
#ifdef RA2_MODE
				if ((i >= 39 && i <= 60) || (i >= 180 && i <= 201) || i == 239 || i == 178 || i == 167 || i == 126
					|| (i >= 122 && i <= 125)) {
					bListIt = FALSE;
				}
#endif

				if (bListIt) {
					CString str;
					str = TranslateStringACP(rules.GetString(id, "Name"));
					Overlays->SetItemData(Overlays->AddString(str), e);
				}
			}
			e++;
		}
	}
}


DWORD CTerrainDlg::GetTileID(DWORD dwTileSet, int iTile)
{
	int i, e;
	DWORD tilecount = 0;
	for (i = 0; i < 10000; i++) {
		CString tset;
		char c[50];
		itoa(i, c, 10);
		int e;
		for (e = 0; e < 4 - strlen(c); e++) {
			tset += "0";
		}
		tset += c;
		CString sec = "TileSet";
		sec += tset;

		auto const pSec = tiles->TryGetSection(sec);
		if (!pSec) {
			return 0xFFFFFFFF;
		}

		auto const tilesInset = pSec->GetInteger("TilesInSet");
		for (e = 0; e < tilesInset; e++) {
			if (i == dwTileSet && e == iTile) {
				return tilecount;
			}
			tilecount++;

		}


	}

	return tilecount;

}


void CTerrainDlg::OnSelchangeOverlay()
{
	auto Overlay = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_OVERLAY));
	//TileSet->GetLBText(TileSet->GetCurSel(), currentTileSet);
	int n = Overlay->GetCurSel();
	if (n < 0) {
		return;
	}

	int sel = Overlay->GetItemData(n);

	auto const frame = reinterpret_cast<CTileSetBrowserFrame*>(GetParentFrame());
	frame->m_view.SetOverlay(sel);
	frame->RecalcLayout();
}

void CTerrainDlg::OnSelchangeTileSetGroup()
{
	if (CTerrainGroupManager::Groups().empty()) {
		return;
	}
	handleTiles();
}

void CTerrainDlg::handleTileGroups()
{
	CTerrainGroupManager::LoadTerrainGroups(Map->GetTheater());
	auto const& TerrainSorts = CTerrainGroupManager::Groups();
	if (TerrainSorts.empty()) {
		errstream << "Waring: terrain group data is empty" << endl;
		return;
	}

	auto cbTerrainGroup = reinterpret_cast<CComboBox*>(GetDlgItem(IDC_TERRAINBAR_TGROUP));
	cbTerrainGroup->ResetContent();

	// always allow clear filter, so we use 'All' as the first item
	cbTerrainGroup->SetItemData(
		cbTerrainGroup->AddString(GetLanguageStringACP("All")),
		-1);

	for (int idx = 0; idx < TerrainSorts.size(); ++idx) {
		auto const& groupN = TerrainSorts[idx];
		// skip empty group
		if (!groupN.Size()) {
			continue;
		}
		cbTerrainGroup->SetItemData(
			cbTerrainGroup->AddString(groupN.Name()),
			idx);
		;
	}
}
