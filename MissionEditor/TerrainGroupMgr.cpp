#include "StdAfx.h"
#include "TerrainGroupMgr.h"
#include "variables.h"
#include "functions.h"
#include "IniHelper.h"

std::vector<TerrainSort> TerrainSorts;
CIniFile terrainGroupData;


CString CTerrainGroupManager::GetControlDataPath()
{
    CString defPath;
    defPath.Format("%s\\%s", AppPath, "TileGroups.ini");
    return g_project.GetStringOr("General", "TileGroupPath", defPath);
}

CTerrainGroupManager::CTerrainGroupManager(CWnd* pParent) :
    CDialog(IDD, pParent)
{

}

void CTerrainGroupManager::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_TERRAIN_MGR_GROUPS, m_groups);
    DDX_Control(pDX, IDC_TERRAIN_MGR_TILES, m_tiles);
}

void CTerrainGroupManager::translateUI()
{
    TranslateWindowCaption(*this, "TerrainGroupMgrTitle");

    TranslateDlgItem(*this, IDOK, "OK");
    TranslateDlgItem(*this, IDCANCEL, "Cancel");
}


// static
const TerrainGroups& CTerrainGroupManager::Groups()
{
    return TerrainSorts;
}

void CTerrainGroupManager::LoadTerrainGroups(CString Theater)
{
    terrainGroupData.LoadFile(CTerrainGroupManager::GetControlDataPath());
    if (terrainGroupData.Size() == 0 || Theater.IsEmpty()) {
        return;
    }

    Theater.MakeLower();
    Theater.SetAt(0, std::toupper(Theater[0])); // camelCase
    CString IniSectionName = "Terrain" + Theater;

    TerrainSorts.clear();
    auto& ini = terrainGroupData;

    int Count = ini.GetInteger(IniSectionName, "Counts");
    if (Count <= 0) {
        return;
    }
    errstream << Count << " Terrain Groups Loading" << endl;
    TerrainSorts.reserve(Count);

    CString indexStr;
    for (int i = 1; i <= Count; ++i) {
        indexStr.Format("%d", i);
        auto const str = ini.GetString(IniSectionName, indexStr);
        TerrainSorts.emplace_back(INIHelper::Split(str, ','));
    }

#if 0
    HWND TerrainWnd = CTileSetDialogBarExt::Instance->GetSafeHwnd();
    HWND ComboMain = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Main);

    ::SendMessageA(ComboMain, CB_RESETCONTENT, NULL, NULL);
    for (int idx = 0; idx < Count; ++idx) {
        ::SendMessageA(ComboMain, CB_ADDSTRING, NULL, (LPARAM)(TerrainSorts[idx].GetName().c_str()));
    }

    ::SendMessageA(ComboMain, CB_SETCURSEL, 0, NULL);
    ::SendMessageA(TerrainWnd, WM_COMMAND, MAKEWPARAM(IDC_TerrainListWindow::ComboBox_Main, CBN_SELCHANGE), (LPARAM)ComboMain);
#endif
    return;
}

void CTerrainGroupManager::SaveTerrainGroups(CString Theater)
{
    auto& ini = terrainGroupData;
    auto& sec = ini.AddSection("Terrain" + Theater);
    auto const count = TerrainSorts.size();
    CString indexStr;
    auto idx = 1ull;
    for (auto const& item : TerrainSorts) {
        indexStr.Format("%d", idx++);
        sec.SetString(indexStr, item.ToString());
    }
    sec.SetInteger("Counts", count);
    ini.SaveFile(GetControlDataPath());
}


#pragma region TerrainSort
//Class TerrainSort
TerrainSort::TerrainSort() {
    Data.clear();
}

TerrainSort::TerrainSort(std::vector<CString>&& init) {
    name = init[0];//group name
    int count = atoi(init[1]);
    if (!count) {
        return;
    }

    for (auto i = 0ull; i < count; ++i) {
        auto displayName = init[i * 2 + 2];
        auto setId = init[i * 2 + 3];
        displayName.Trim();
        setId.Trim();
        // displayName is more of debug use
        Data.emplace(std::move(setId), std::move(displayName));
    }
    init.clear();
}

const CString TerrainSort::ToString() const
{
    // TODO, serialize
    return CString();
}

#pragma endregion