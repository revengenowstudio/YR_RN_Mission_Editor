#pragma once

#include <afxcview.h>
#include <functional>

using HMIXFILE = DWORD;
using FindFileInMixHandler = std::function<HMIXFILE(LPCTSTR lpFilename, char* pTheaterChar)>;

struct VoxelRectangle
{
	int X, Y, W, H;
};

class VoxelDrawer
{
public:
	static void Initalize();
	static void Finalize();
	static bool LoadVPLFile(const CString& name, const FindFileInMixHandler& mixFindler);
	static bool LoadVXLFile(const CString& name, const FindFileInMixHandler& mixFindler);
	static bool LoadHVAFile(const CString& name, const FindFileInMixHandler& mixFindler);
	static bool GetImageData(unsigned int nFacing, unsigned char*& pBuffer,
		int& width, int& height, int& x, int& y, const int F = 0, const int L = 0, const int H = 0);
	static bool GetImageData(unsigned int nFacing, unsigned char*& pBuffer,
		VoxelRectangle& rect, const int F = 0, const int L = 0, const int H = 0);
	static bool IsVPLLoaded();
};