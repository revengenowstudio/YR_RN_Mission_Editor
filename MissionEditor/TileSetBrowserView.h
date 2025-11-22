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

#if !defined(AFX_TILESETBROWSERVIEW_H__3DD92021_7D37_11D4_9C87_97337B61A44A__INCLUDED_)
#define AFX_TILESETBROWSERVIEW_H__3DD92021_7D37_11D4_9C87_97337B61A44A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileSetBrowserView.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Ansicht CTileSetBrowserView 

class CTileSetBrowserView : public CScrollView
{
public:
	CTileSetBrowserView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	virtual ~CTileSetBrowserView();
	DECLARE_DYNCREATE(CTileSetBrowserView)

	// Attribute

	// Operationen

	// Implementierung
	void SetOverlay(DWORD dwID);
	int GetAddedHeight(DWORD dwID);
	void DrawIt();
	void SetTileSet(DWORD dwTileSet, BOOL bOnlyRedraw = FALSE);
	DWORD GetTileID(DWORD dwTileSet, DWORD dwType);
	const auto BottomNeeded() const { return m_bottom_needed; }
	const auto CurrentTileSet() const { return m_currentTileSet; }
	void RecalcBottomNeeded();

protected:
	enum class PlaceMode : unsigned
	{
		None,
		TileSet,
		Overlay,
	};

	virtual void OnDraw(CDC* pDC) override;      // Überschrieben zum Zeichnen dieser Ansicht
	virtual void OnInitialUpdate() override;     // Zum ersten Mal nach der Konstruktion
	virtual void PostNcDestroy() override;

	void onDrawTileSetPlacement(CDC* pDC);
	void onDrawOverlayPlacement(CDC* pDC);

	void onPlaceTileSet(const CPoint point, int max_r);
	void onPlaceOverlay(const CPoint point, int max_r);

#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif
	LPDIRECTDRAWSURFACE7 RenderTile(DWORD dwID);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

	PlaceMode m_CurrentMode{ PlaceMode::None };
	int m_tilecount{ 0 };
	int m_tile_height{ 1 };
	int m_tile_width{ 1 };
	int m_currentTileSet{ -1 };
	int m_bottom_needed{ 1000 };
	int m_currentOverlay{ -1 };
	LPDIRECTDRAWSURFACE7* m_lpDDS{ nullptr };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_TILESETBROWSERVIEW_H__3DD92021_7D37_11D4_9C87_97337B61A44A__INCLUDED_
