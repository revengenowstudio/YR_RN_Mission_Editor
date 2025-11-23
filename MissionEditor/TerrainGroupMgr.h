#pragma once
#include <afxrich.h>
#include <unordered_map>
#include "IniFile.h"

class TerrainSort 
{
public:
    TerrainSort();
    TerrainSort(std::vector<CString>&& init);

    bool Contains(const CString& tileSetId) const { return Data.find(tileSetId) != Data.end();  }
    auto Size() const { return Data.size(); }

    const CString ToString() const;

    CString Name() const { return name; }

private:
    // tileset-Id : displayName
    std::unordered_map<CString, CString, CStringHash> Data;
    CString name; // group display name
};
using TerrainGroups = std::vector<TerrainSort>;

class CTerrainGroupManager : public CDialog
{
public:
    enum { IDD = IDD_TERRAIN_MGR };

    static CString GetControlDataPath();
    static const TerrainGroups& Groups();
    static void LoadTerrainGroups(CString Theater);

    CTerrainGroupManager(CWnd* pParent);

    virtual void DoDataExchange(CDataExchange* pDX) override;

private:
    static void SaveTerrainGroups(CString Theater);

    void translateUI();

    CListBox m_groups;
    CListBox m_tiles;
};
