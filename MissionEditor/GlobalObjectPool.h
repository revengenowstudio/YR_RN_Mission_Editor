#pragma once
#ifndef GLOBAL_OBJECT_POOL_H
#define GLOBAL_OBJECT_POOL_H
#include "Defines.h"
#include "Structs.h"
#include "IniHelper.h"
#include <unordered_map>

class OverlayCache
{
public:
    void ResetAll();

    void Set(size_t overlayId, size_t subSeq, const PICDATA* pData)
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
    const PICDATA* ovrlpics[OverlayCacheSlots][max_ovrl_img];
};

class ImageCache
{
public:
    const PICDATA* Read(const CString& key) const
    {
        auto const it = pics.find(key);
        if (it != pics.end()) {
            return &it->second;
        }
        return nullptr;
    }
    PICDATA& Acquire(const CString& key)
    {
        auto const [it, _] = pics.try_emplace(key, PICDATA());
        return it->second;
    }
    bool Exists(const CString& key) const { return pics.find(key) != pics.end(); }

    auto Size() const { return pics.size(); }

    void ResetAll();

private:
    std::unordered_map<CString, PICDATA, CStringHash> pics;
};

class GlobalObjectPool
{
public:
    static GlobalObjectPool& Instance();

    auto& Overlays() { return overlays; }
    auto& Images() { return images; }

private:
    OverlayCache overlays;
    // all the pictures shown in the mapview
    ImageCache images;
};

#endif