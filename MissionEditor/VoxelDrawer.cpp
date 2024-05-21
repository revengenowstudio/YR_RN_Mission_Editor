#include "stdafx.h"
#include "VoxelDrawer.h"
#include <vxlapi.h>
#include "variables.h"
#include "MissionEditorPackLib.h"

CString makeFullPath(const CString& name)
{
	return TSPath + name;
}

std::pair<MemoryBuffer, bool> readFile(const CString& name, HMIXFILE hMix)
{
	if (hMix == NULL) {
		return FSunPackLib::LoadCCFile(makeFullPath(name), NULL);
	}
	return FSunPackLib::LoadCCFile(name, hMix);
}

void VoxelDrawer::Initalize()
{
	CncImgCreate();
	CncImgSetMaxFacing(32);
}

void VoxelDrawer::Finalize()
{
	CncImgRelease();
}

bool VoxelDrawer::LoadVPLFile(const CString& name, const FindFileInMixHandler& mixFindler)
{
	bool result = false;
	auto const&& ret = readFile(name, mixFindler(name, nullptr));
	if (ret.second) {
		result = CncImgLoadVPLFile(reinterpret_cast<const BYTE*>(ret.first.data()));
	}
	return result;
}

bool VoxelDrawer::LoadVXLFile(const CString& name, const FindFileInMixHandler& mixFindler)
{
	bool result = false;
	auto const&& ret = readFile(name, mixFindler(name, nullptr));
	if (ret.second) {
		if (CncImgIsVXLLoaded()) {
			CncImgClearCurrentVXL();
		}
		result = CncImgLoadVXLFile(reinterpret_cast<const BYTE*>(ret.first.data()));
	}
	return result;
}

bool VoxelDrawer::LoadHVAFile(const CString& name, const FindFileInMixHandler& mixFindler)
{
	bool result = false;
	auto const&& ret = readFile(name, mixFindler(name, nullptr));
	if (ret.second) {
		result = CncImgLoadHVAFile(reinterpret_cast<const BYTE*>(ret.first.data()));
	}
	return result;
}

bool VoxelDrawer::GetImageData(unsigned int nFacing, unsigned char*& pBuffer, int& width,
	int& height, int& x, int& y, const int F, const int L, const int H)
{
	const unsigned int nIndex = nFacing * 4;
	CncImgPrepareVXLCache(nIndex, F, L, H);
	CncImgGetImageFrame(nIndex, &width, &height, &x, &y);
	if (width < 0 || height < 0)
		return false;
	return CncImgGetImageData(nIndex, &pBuffer);
}

bool VoxelDrawer::GetImageData(unsigned int nFacing, unsigned char*& pBuffer, VoxelRectangle& rect, const int F, const int L, const int H)
{
	return GetImageData(nFacing, pBuffer, rect.W, rect.H, rect.X, rect.Y, F, L, H);
}

bool VoxelDrawer::IsVPLLoaded()
{
	return CncImgIsVPLLoaded();
}
