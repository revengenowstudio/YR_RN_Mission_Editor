#pragma once
#ifndef GLOBAL_OBJECT_POOL_H
#define GLOBAL_OBJECT_POOL_H
#include "Defines.h"
#include "Structs.h"

class OverlayCache
{
public:
    void ResetAll();

    void Set(size_t overlayId, size_t subSeq, PICDATA* pData)
    {
        // TODO: boundary check
        ovrlpics[overlayId][subSeq] = pData;
    }
    const PICDATA* Read(size_t overlayId, size_t subSeq) const
    {
        // TODO: boundary check
        return ovrlpics[overlayId][subSeq];
    }

private:
    static auto constexpr OverlayCacheSlots = 0x1000ull;

    /* Overlay picture table (maximum overlay count=0xFF) */
    PICDATA* ovrlpics[OverlayCacheSlots][max_ovrl_img];
};

class GlobalObjectPool
{
public:
    static GlobalObjectPool& Instance();

    auto& Overlays() { return overlays; }

private:
    OverlayCache overlays;
};

#endif