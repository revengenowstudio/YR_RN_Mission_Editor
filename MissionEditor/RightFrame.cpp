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

// RightFrame.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "finalsun.h"
#include "RightFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightFrame

IMPLEMENT_DYNCREATE(CRightFrame, CFrameWnd)

CRightFrame::CRightFrame()
{
}

CRightFrame::~CRightFrame()
{
}


BEGIN_MESSAGE_MAP(CRightFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CRightFrame)
	ON_WM_SIZE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CRightFrame 

BOOL CRightFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect rect;
	rect.right = 200;
	rect.bottom = 200;

	if (!m_Splitter.CreateStatic(this, 1, 2)) {
		return FALSE;
	}

	if (!m_Splitter.CreateView(0, 0,
		RUNTIME_CLASS(CIsoView),
		{ 700,200 },
		pContext)) {
		return FALSE;
	}

	if (!m_Splitter.CreateView(0, 1,
		RUNTIME_CLASS(CTileSetBrowserFrame),
		{ 200,200 },
		pContext)) {
		return FALSE;
	}

	GetClientRect(&rect);

	auto const oct = GetSystemMetrics(SM_CXFULLSCREEN) / 8;
	m_Splitter.SetColumnInfo(0, 5 * oct, 20);
	m_Splitter.SetColumnInfo(1, 3 * oct, 10);

	//SetWindowLong(m_Splitter->m_hWnd, GWL_STYLE, m_rightFrame->GetStyle() ! WS_THICKFRAME);

	OutputDebugString("CRightFrame::OnCreateClient(): windows created\n");

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CRightFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// cs.style=WS_OVERLAPPED && WS_CAPTION && WS_SYSMENU && WS_MINIMIZEBOX && WS_MAXIMIZEBOX;

	return CFrameWnd::PreCreateWindow(cs);
}

void CRightFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);


}

void CRightFrame::RecalcLayout(BOOL bNotify)
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen

	CFrameWnd::RecalcLayout(bNotify);
}

void CRightFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);
}
