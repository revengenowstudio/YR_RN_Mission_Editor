#pragma once
#include <afxrich.h>
#include <vector>

class TerrainSort {
public:
    TerrainSort();
    TerrainSort(std::vector<CString>&& init);

    auto const& operator[](int index) const
    {
        return Data.at(index);
    }

    const CString ToString() const;
    auto Count() const { return Data.size(); }
    void Resize(size_t size) { Data.resize(size); }
    CString Name() const { return name; }

private:
    std::vector<std::pair<CString, CString>> Data;
    CString name;
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
