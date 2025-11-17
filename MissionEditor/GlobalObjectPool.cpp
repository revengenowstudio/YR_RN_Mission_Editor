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
