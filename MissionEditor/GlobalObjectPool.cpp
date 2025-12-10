#include "stdafx.h"
#include "GlobalObjectPool.h"
#include <ddraw.h>

GlobalObjectPool& GlobalObjectPool::Instance()
{
    static GlobalObjectPool inst; // thread safe init, modern cpp guarantee
    return inst;
}

void OverlayCache::ResetAll()
{
    ZeroMemory(ovrlpics, sizeof(ovrlpics));

}

void ImageCache::ResetAll()
{

	int e = 0;
	for (auto it = pics.begin(); it != pics.end(); it++, e++) {
		try {
#ifdef NOSURFACES_OBJECTS			
			if (it->second.bType == PICDATA_TYPE_BMP) {
				if (auto pPic = std::exchange(it->second.pic, nullptr)) {
					((LPDIRECTDRAWSURFACE7)pPic)->Release();
				}
			}
			else {
				if (auto pPic = std::exchange(it->second.pic, nullptr)) {
					delete[](pPic);
				}
				if (auto pBorder = std::exchange(it->second.vborder, nullptr)) {
					delete[](pBorder);
				}
			}
#else
			if (i->second.pic != NULL) i->second.pic->Release();
#endif

			it->second.pic = NULL;
		}
		catch (...) {
			CString err;
			err = "Access violation while trying to release surface ";
			char c[6];
			itoa(e, c, 10);
			err += c;

			err += "\n";
			OutputDebugString(err);
			continue;
		}
	}
	pics.clear();
}
