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

#if !defined(AFX_VIEWOBJECTS_H__360F4320_9B82_11D3_B63B_EC44EDA1D441__INCLUDED_)
#define AFX_VIEWOBJECTS_H__360F4320_9B82_11D3_B63B_EC44EDA1D441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewObjects.h : Header-Datei
//
#include <afxcview.h>
#include <unordered_set>

#define MAKE_MASK(refVal) 1 << static_cast<int>(refVal) 

class TreeViewBuilder;
class CViewObjects;
class IniFileGroup;

using IgnoreSet = std::unordered_set<std::string>;

static const IgnoreSet CollectIgnoreSet();

extern CIniFile rules;

class TreeRoot {
	friend class TreeViewBuilder;
	friend class CViewObjects;
	enum _ : int {
		Nothing = -1,
		Infantry,
		Vehicle,
		Aircraft,
		Building,
		Terrain,
		Overlay,
		Waypoint,
		Celltag,
		Basenode,
		Tunnel,
		Delete,
		Owner,
		PlayerLocation,
		Ground,
		Smudge,
		Count,
		Begin = 0,
	};
};

enum class TreeViewTechnoType {
	Set_None = -1,
	Building = 0,
	Infantry,
	Vehicle,
	Aircraft,
	_Last,
	_First = 0,
	Count = _Last,
};

enum class TechnoTypeMask
{
	ForBuilding = MAKE_MASK(TreeViewTechnoType::Building),
	ForInfantry = MAKE_MASK(TreeViewTechnoType::Infantry),
	ForVehicle = MAKE_MASK(TreeViewTechnoType::Vehicle),
	ForAircraft = MAKE_MASK(TreeViewTechnoType::Aircraft),
};

inline bool operator&(TechnoTypeMask lhs, TechnoTypeMask rhs)
{
	return static_cast<int>(lhs) & static_cast<int>(rhs);
}
inline bool operator&(TechnoTypeMask lhs, TreeViewTechnoType rhs)
{
	return lhs & static_cast<TechnoTypeMask>(MAKE_MASK(rhs));
}
inline bool operator&(TreeViewTechnoType lhs, TechnoTypeMask rhs)
{
	return rhs & static_cast<TechnoTypeMask>(MAKE_MASK(lhs));
}

class GuessSideHelper {
public:
	GuessSideHelper(const TreeViewBuilder& builder) : 
		builder(builder)
	{}

	const CString& GetSideName(const CString& regName, TreeViewTechnoType technoType, const CIniFile& inWhichIni = rules);
	int GuessSide(const CString& pRegName, TreeViewTechnoType nType, const CIniFile& inWhichIni);
	int guessGenericSide(const CString& typeId);

private:
	int guessBuildingSide(const CString& typeId, const CIniFile& inWhichIni);

	std::unordered_map<std::string, int> KnownItem;
	const TreeViewBuilder& builder;
};

class TreeViewBuilder {
	using houseMap = std::unordered_map<std::string, int>;
public:
	struct CatetoryDefinition {
		CString CategoryName;// or side name
		TechnoTypeMask CategoryMask;
	};

	using mapSideNodeInfo = std::map<int, CatetoryDefinition>;

	TreeViewBuilder(CTreeCtrl& tree,
		const CIniFile& ini,
		const IgnoreSet& ignoreSet,
		const CString& theater,
		const TheaterChar needed_terrain,
		const HTREEITEM rootitems[]) :
		tree(tree),
		ini(ini),
		theater(theater),
		needed_terrain(needed_terrain),
		rootitems(rootitems),
		m_ignoreSet(ignoreSet),
		m_owners(collectOwners()),
		sideInfo(collectCategoryInfo())
	{
		updateTechnoItems();
	}

	const houseMap m_owners;
	const mapSideNodeInfo sideInfo;

private:
	static mapSideNodeInfo collectCategoryInfo();
	static const houseMap collectOwners();
	void updateTechnoItems();
	void updateBuildingTypes(HTREEITEM parentNode);
	void updateUnitTypes(HTREEITEM parentNode, const char* typeListId, TreeViewTechnoType technoType, int multiple);

	const IgnoreSet& m_ignoreSet;
	CTreeCtrl& tree;
	const CIniFile& ini;
	const CString& theater;
	const TheaterChar needed_terrain;
	const HTREEITEM* rootitems;
};

class TreeViewCategoryHandler {
	using categoryNodeMap = std::unordered_map<std::string, HTREEITEM>;

public:
	TreeViewCategoryHandler(CTreeCtrl& tree, HTREEITEM parentNode) :
		tree(tree),
		parentNode(parentNode)
	{}

	void Preallocate(const TreeViewBuilder::mapSideNodeInfo& sideInfo, TreeViewTechnoType type);
	HTREEITEM GetOrAdd(const CString& name);

private:
	CTreeCtrl& tree;
	HTREEITEM parentNode;
	categoryNodeMap structhouses;
};

/////////////////////////////////////////////////////////////////////////////
// Ansicht CViewObjects 

class CViewObjects : public CTreeView
{
protected:
	CViewObjects();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CViewObjects)

	// Attribute
public:

	// Operationen
public:
	void UpdateDialog();

	// Überschreibungen
		// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
		//{{AFX_VIRTUAL(CViewObjects)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnDraw(CDC* pDC);      // Überschrieben zum Zeichnen dieser Ansicht
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CViewObjects();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CViewObjects)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void HandleBrushSize(int iTile);
};

class IniMegaFile
{
	friend class IniFileGroup;
public:
	static IniFileGroup GetRules();

	static bool IsNullOrEmpty(const  CString& value) { return isNullOrEmpty(value); }

private:
	static bool isNullOrEmpty(const CString& value);


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_VIEWOBJECTS_H__360F4320_9B82_11D3_B63B_EC44EDA1D441__INCLUDED_
