#include "stdafx.h"
#include "Palettes.h"
#include "variables.h"
#include "functions.h"

void Palettes::Init()
{
    if (!FSunPackLib::XCC_ExtractFile("isotem.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "IsoTem.pal failed\n";
    m_hPalIsoTemp = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("isosno.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "IsoSno.pal failed\n";
    m_hPalIsoSnow = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("isourb.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "IsoUrb.pal failed\n";
    m_hPalIsoUrb = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    HMIXFILE m_hCache2 = loading.ExpandMixes()[100].hECache;
    if (!FSunPackLib::XCC_ExtractFile("isolun.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "IsoLun.pal failed\n";
    m_hPalIsoLun = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("isodes.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "IsoDes.pal failed\n";
    m_hPalIsoDes = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("isoubn.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "IsoUbn.pal failed\n";
    m_hPalIsoUbn = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");


    if (!FSunPackLib::XCC_ExtractFile("unittem.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "UnitTem.pal failed";
    m_hPalUnitTemp = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("unitsno.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "UnitSno.pal failed\n";
    m_hPalUnitSnow = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("uniturb.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "UnitUrb.pal failed\n";
    m_hPalUnitUrb = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");


    if (!FSunPackLib::XCC_ExtractFile("unitlun.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "UnitLun.pal failed\n";
    m_hPalUnitLun = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("unitdes.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "UnitDes.pal failed\n";
    m_hPalUnitDes = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("unitubn.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "UnitUbn.pal failed\n";
    m_hPalUnitUbn = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");


    if (!FSunPackLib::XCC_ExtractFile("snow.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "Snow.pal failed\n";
    m_hPalSnow = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("temperat.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "Temperat.pal failed\n";
    m_hPalTemp = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("urban.pal", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "Urban.pal failed\n";
    m_hPalUrb = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");


    if (!FSunPackLib::XCC_ExtractFile("lunar.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "lunar.pal failed\n";
    m_hPalLun = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("desert.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "desert.pal failed\n";
    m_hPalDes = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    if (!FSunPackLib::XCC_ExtractFile("urbann.pal", u8AppDataPath + "\\TmpPalette.pal", m_hCache2))
        errstream << "urbann.pal failed\n";
    m_hPalUbn = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");


    if (!FSunPackLib::XCC_ExtractFile("_ID2124019542", u8AppDataPath + "\\TmpPalette.pal", loading.CacheMix()))
        errstream << "lib.pal failed\n";
    m_hPalLib = FSunPackLib::LoadTSPalette(u8AppDataPath + "\\TmpPalette.pal", NULL);
    deleteFile(u8AppDataPath + "\\TmpPalette.pal");

    LoadedPalettes["isotem.pal"] = new Palette(m_hPalIsoTemp);
    LoadedPalettes["isosno.pal"] = new Palette(m_hPalIsoSnow);
    LoadedPalettes["isourb.pal"] = new Palette(m_hPalIsoUrb);
    LoadedPalettes["isoubn.pal"] = new Palette(m_hPalIsoUbn);
    LoadedPalettes["isolun.pal"] = new Palette(m_hPalIsoLun);
    LoadedPalettes["isodes.pal"] = new Palette(m_hPalIsoDes);
    
    LoadedPalettes["unittem.pal"] = new Palette(m_hPalUnitTemp, true);
    LoadedPalettes["unitsno.pal"] = new Palette(m_hPalUnitSnow, true);
    LoadedPalettes["uniturb.pal"] = new Palette(m_hPalUnitUrb, true);
    LoadedPalettes["unitubn.pal"] = new Palette(m_hPalUnitUbn, true);
    LoadedPalettes["unitlun.pal"] = new Palette(m_hPalUnitLun, true);
    LoadedPalettes["unitdes.pal"] = new Palette(m_hPalUnitDes, true);

    LoadedPalettes["temperat.pal"] = new Palette(m_hPalTemp);
    LoadedPalettes["snow.pal"] = new Palette(m_hPalSnow);
    LoadedPalettes["urban.pal"] = new Palette(m_hPalUrb);
    LoadedPalettes["urbann.pal"] = new Palette(m_hPalUbn);
    LoadedPalettes["lunar.pal"] = new Palette(m_hPalLun);
    LoadedPalettes["desert.pal"] = new Palette(m_hPalDes);
    LoadedPalettes["lib.pal"] = new Palette(m_hPalLib);
}


HTSPALETTE Palettes::GetIsoPalette(char theat)
{
    HTSPALETTE isoPalette = m_hPalIsoTemp;
    switch (theat) {
    case 'T':
    case 'G':
        isoPalette = m_hPalIsoTemp;
        break;
    case 'A':
        isoPalette = m_hPalIsoSnow;
        break;
    case 'U':
        isoPalette = m_hPalIsoUrb;
        break;
    case 'N':
        isoPalette = m_hPalIsoUbn;
        break;
    case 'D':
        isoPalette = m_hPalIsoDes;
        break;
    case 'L':
        isoPalette = m_hPalIsoLun;
        break;
    }
    return isoPalette;
}

HTSPALETTE Palettes::GetUnitPalette(char theat)
{
    HTSPALETTE isoPalette = m_hPalUnitTemp;
    switch (theat) {
    case 'T':
    case 'G':
        isoPalette = m_hPalUnitTemp;
        break;
    case 'A':
        isoPalette = m_hPalUnitSnow;
        break;
    case 'U':
        isoPalette = m_hPalUnitUrb;
        break;
    case 'N':
        isoPalette = m_hPalUnitUbn;
        break;
    case 'D':
        isoPalette = m_hPalUnitDes;
        break;
    case 'L':
        isoPalette = m_hPalUnitLun;
        break;
    }
    return isoPalette;
}


/*
Helper function that fetches the palette data from FsunPackLib
FSunPackLib doesn´t provide any special function to retrieve a color table entry,
so we have to build it ourself
Also builds color_conv
*/
void Palettes::FetchPalettes()
{
    // SetTSPaletteEntry(HTSPALETTE hPalette, BYTE bIndex, RGBTRIPLE* rgb, RGBTRIPLE* orig);
    // SetTSPaletteEntry can retrieve the current color table entry without modifying it!


    // iso palette
    HTSPALETTE hCur = 0;
    if (Map->GetTheater() == THEATER0) hCur = m_hPalIsoTemp;
    if (Map->GetTheater() == THEATER1) hCur = m_hPalIsoSnow;
    if (Map->GetTheater() == THEATER2) hCur = m_hPalIsoUrb;
    if (Map->GetTheater() == THEATER3) hCur = m_hPalIsoUbn;
    if (Map->GetTheater() == THEATER4) hCur = m_hPalIsoLun;
    if (Map->GetTheater() == THEATER5) hCur = m_hPalIsoDes;

    int i;

    for (i = 0; i < 256; i++) {
        FSunPackLib::SetTSPaletteEntry(hCur, i, NULL /* don´t modify it!*/, &palIso[i] /*but retrieve it!*/);
    }


    // unit palette
    if (Map->GetTheater() == THEATER0) hCur = m_hPalUnitTemp;
    if (Map->GetTheater() == THEATER1) hCur = m_hPalUnitSnow;
    if (Map->GetTheater() == THEATER2) hCur = m_hPalUnitUrb;
    if (Map->GetTheater() == THEATER3) hCur = m_hPalUnitUbn;
    if (Map->GetTheater() == THEATER4) hCur = m_hPalUnitLun;
    if (Map->GetTheater() == THEATER5) hCur = m_hPalUnitDes;


    for (i = 0; i < 256; i++) {
        FSunPackLib::SetTSPaletteEntry(hCur, i, NULL /* don´t modify it!*/, &palUnit[i] /*but retrieve it!*/);
    }


    // theater palette
    if (Map->GetTheater() == THEATER0) hCur = m_hPalTemp;
    if (Map->GetTheater() == THEATER1) hCur = m_hPalSnow;
    if (Map->GetTheater() == THEATER2) hCur = m_hPalUrb;
    if (Map->GetTheater() == THEATER3) hCur = m_hPalUbn;
    if (Map->GetTheater() == THEATER4) hCur = m_hPalLun;
    if (Map->GetTheater() == THEATER5) hCur = m_hPalDes;



    for (i = 0; i < 256; i++) {
        FSunPackLib::SetTSPaletteEntry(hCur, i, NULL /* don´t modify it!*/, &palTheater[i] /*but retrieve it!*/);
    }


    // lib palette
    hCur = m_hPalLib;


    for (i = 0; i < 256; i++) {
        FSunPackLib::SetTSPaletteEntry(hCur, i, NULL /* don´t modify it!*/, &palLib[i] /*but retrieve it!*/);
    }

    CreateConvTable(palIso, iPalIso);
    CreateConvTable(palLib, iPalLib);
    CreateConvTable(palUnit, iPalUnit);
    CreateConvTable(palTheater, iPalTheater);

    CIsoView& v = *((CFinalSunDlg*)theApp.m_pMainWnd)->m_view.m_isoview;

    DDPIXELFORMAT pf;
    memset(&pf, 0, sizeof(DDPIXELFORMAT));
    pf.dwSize = sizeof(DDPIXELFORMAT);

    v.lpds->GetPixelFormat(&pf);
    v.pf = pf;
    v.m_color_converter.reset(new FSunPackLib::ColorConverter(v.pf));

    FSunPackLib::ColorConverter conf(pf);

    for (auto const& [name, col] : rules["Colors"]) {
        COLORREF cref = v.GetColor("", col);

        color_conv[col] = conf.GetColor(GetRValue(cref), GetGValue(cref), GetBValue(cref));
        colorref_conv[cref] = color_conv[col];
    }
}

void Palettes::CreateConvTable(RGBTRIPLE* pal, int* iPal)
{
    CIsoView& v = *((CFinalSunDlg*)theApp.m_pMainWnd)->m_view.m_isoview;

    DDPIXELFORMAT pf;
    memset(&pf, 0, sizeof(DDPIXELFORMAT));
    pf.dwSize = sizeof(DDPIXELFORMAT);

    v.lpds->GetPixelFormat(&pf);

    FSunPackLib::ColorConverter conf(pf);

    int i;
    for (i = 0; i < 256; i++) {
        iPal[i] = conf.GetColor(pal[i].rgbtRed, pal[i].rgbtGreen, pal[i].rgbtBlue);
    }
}


Palette* Palettes::LoadPalette(const CString& palName)
{
    if (LoadedPalettes.size() == 0) {
        Palettes::Init();
    }

    auto it = LoadedPalettes.find(palName);
    if (it != LoadedPalettes.end()) {
        return it->second;
    }

    BytePalette buffer;
    HMIXFILE mixIdx = loading.FindFileInMix(palName);

    if (FSunPackLib::LoadTSPalette(reinterpret_cast<RGBTRIPLE*>(buffer.Data), palName, mixIdx)) {
        auto pal = new Palette(buffer);
        LoadedPalettes[palName] = pal;
        return pal;
    }

    return nullptr;
}

void Palettes::Clear()
{
    for (auto& pair : LoadedPalettes) {
        delete(pair.second);
    }
    LoadedPalettes.clear();
    RemappedPalettes.clear();
    Init();
}

Palette::Palette(const BytePalette& bytes, bool remappable) :
    Remappable(remappable)
{
    for (auto idx = 0; idx < 256; idx++) {
        Data[idx].R = bytes.Data[idx].red;
        Data[idx].G = bytes.Data[idx].green;
        Data[idx].B = bytes.Data[idx].blue;
        Data[idx].Zero = 0;
    }
}

Palette::Palette(HTSPALETTE indexer, bool remappable) :
    Remappable(remappable)
{
    for (auto idx = 0; idx < 256; idx++) {
        RGBTRIPLE ret;
        FSunPackLib::SetTSPaletteEntry(indexer, idx, NULL /* don´t modify it!*/, &ret /*but retrieve it!*/);
        Data[idx].R = ret.rgbtRed;
        Data[idx].G = ret.rgbtGreen;
        Data[idx].B = ret.rgbtBlue;
        Data[idx].Zero = 0;
    }
}
