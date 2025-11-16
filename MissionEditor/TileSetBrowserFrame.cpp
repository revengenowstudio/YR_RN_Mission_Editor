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

// TileSetBrowserFrame.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "finalsun.h"
#include "TileSetBrowserFrame.h"
#include "res/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTileSetBrowserFrame

IMPLEMENT_DYNCREATE(CTileSetBrowserFrame, CFrameWnd)

CTileSetBrowserFrame::CTileSetBrowserFrame()
{
}

CTileSetBrowserFrame::~CTileSetBrowserFrame()
{
}


BEGIN_MESSAGE_MAP(CTileSetBrowserFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CTileSetBrowserFrame)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTileSetBrowserFrame 

void CTileSetBrowserFrame::PostNcDestroy()
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	delete this;
	// CFrameWnd::PostNcDestroy();
}

BOOL CTileSetBrowserFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (!m_bar.CDialogBar::Create(this, IDD_TERRAINBAR, CBRS_TOP, 5)) {
		return FALSE;
	}

	m_bar.TranslateUI();
	m_bar.ShowWindow(SW_SHOW);
	CRect rect;
	GetClientRect(rect);

	m_view.Create(NULL, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE, rect, this, 1, NULL);

	RecalcLayout(TRUE);

	const CSize sizeTotal{ rect.right,  m_view.m_bottom_needed };
	GetClientRect(&rect);

	m_view.SetScrollSizes(MM_TEXT, sizeTotal);

	m_view.ShowWindow(SW_SHOW);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CTileSetBrowserFrame::RecalcLayout(BOOL bNotify)
{

	RECT rect;
	RECT rm;
	GetClientRect(&rm);
	m_bar.GetClientRect(&rect);

	m_view.SetWindowPos(NULL, 0, rect.bottom, rm.right, rm.bottom - rect.bottom, SWP_NOZORDER);

	m_view.GetClientRect(&rect);
	CSize sizeTotal;
	sizeTotal.cx = rect.right;
	sizeTotal.cy = m_view.m_bottom_needed;
	m_view.SetScrollSizes(MM_TEXT, sizeTotal);

	CFrameWnd::RecalcLayout(bNotify);
}

void CTileSetBrowserFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	RecalcLayout();

}

