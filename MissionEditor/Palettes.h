#pragma once

#include <afxcview.h>
#include <map>
#include "MissionEditorPackLib.h"

class CLoading;

struct BGRStruct
{
    unsigned char B, G, R, Zero;
    bool operator< (const BGRStruct& rhs) const { return *(int*)this < *(int*)&rhs; }
    bool operator==(const BGRStruct& rhs) const { return *(int*)this == *(int*)&rhs; }
};

struct ColorStruct
{
    unsigned char red, green, blue;
};

class BytePalette
{
public:
    ColorStruct Data[256];

    ColorStruct& operator[](int index) { return Data[index]; }
};

class Palette
{
public:
    Palette(const BytePalette& bytes, bool remappable = false);
    Palette(HTSPALETTE raw, bool remappable = false);

    BGRStruct& operator[](int index) { return Data[index]; }
    ColorStruct GetByteColor(int index) {
        ColorStruct ret;
        BGRStruct& tmp = Data[index];
        ret.red = tmp.R;
        ret.green = tmp.G;
        ret.blue = tmp.B;
        return ret;
    }
    const BGRStruct* GetData() const { return Data; }
    bool IsRemappable() const { return Remappable; }

private:
    BGRStruct Data[256]{};
    bool Remappable{};
};

class Palettes
{
public:
    Palettes(CLoading& loading) :
        loading(loading)
    {}

    void Init();

    HTSPALETTE GetIsoPalette(char theat);
    HTSPALETTE GetUnitPalette(char theat);
    void FetchPalettes();
    void CreateConvTable(RGBTRIPLE* pal, int* iPal);

    Palette* LoadPalette(const CString& palName);
    void Clear();

    HTSPALETTE m_hPalIsoTemp;
    HTSPALETTE m_hPalIsoSnow;
    HTSPALETTE m_hPalIsoUrb;

    HTSPALETTE m_hPalUnitTemp;
    HTSPALETTE m_hPalUnitSnow;
    HTSPALETTE m_hPalUnitUrb;
    HTSPALETTE m_hPalTemp;
    HTSPALETTE m_hPalSnow;
    HTSPALETTE m_hPalUrb;
    HTSPALETTE m_hPalLib;
    // YR pals:
    HTSPALETTE m_hPalLun;
    HTSPALETTE m_hPalDes;
    HTSPALETTE m_hPalUbn;
    HTSPALETTE m_hPalIsoLun;
    HTSPALETTE m_hPalIsoDes;
    HTSPALETTE m_hPalIsoUbn;
    HTSPALETTE m_hPalUnitLun;
    HTSPALETTE m_hPalUnitDes;
    HTSPALETTE m_hPalUnitUbn;

private:
    CLoading& loading;
    std::map<CString, Palette*> LoadedPalettes;
    std::map<Palette*, std::map<BGRStruct, Palette>> RemappedPalettes;
};