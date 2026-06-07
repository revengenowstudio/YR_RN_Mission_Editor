#include "StdAfx.h"
#include "CommandBridgeClient.h"
#include "Serde.h"
#include "Handler.h"
#include "TriggerDatabase.h"
#include "variables.h"
#include "functions.h"

namespace {

using namespace Serde;

bool IsMapReady() {
    return Map != nullptr;
}

} // namespace

/* ── Query callbacks ───────────────────────────────────────────────────── */

void OnTagList(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto& db = DB::Tags;
    auto count = db.Size();
    auto items = std::make_unique<RPCB_StrView[]>(count);
    size_t n = 0;
    for (size_t i = 0; i < count; ++i) {
        items[n].data = db.Nth(i).id;
        items[n].len  = db.Nth(i).id.GetLength();
        n++;
    }
    SendArrayOK(ctx, items.get(), n);
}

void OnTagGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    auto* tag = DB::Tags.TryLookup(id);
    if (!tag) {
        SendError(ctx, CBError::TagNotFound);
        return;
    }
    CacheBuilder items;
    items.PushKeyValue("id",          tag->id);
    items.PushKeyValue("name",        tag->name);
    items.PushKeyValue("triggerId", tag->triggerId);
    items.PushKeyValue("persistence", tag->persistence);
    SendOK(ctx, items.Build());
}

/* ── Callbacks with params — use ParamExtractor ────────────────────────── */

void OnTagCreate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString name, triggerId;
    int persistence = 0;
    name = "New Tag";
    ParamExtractor ex;
    ex.Add("name",        [&](std::string_view v) { name      = CString(v.data(), v.size()); });
    ex.Add("triggerId",   [&](std::string_view v) { triggerId = CString(v.data(), v.size()); });
    ex.Add("persistence", [&](std::string_view v) { persistence = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);

    TagInstance tag(GetFreeID());
    tag.name        = name;
    tag.triggerId   = triggerId;
    tag.persistence = persistence;
    DB::Tags.Append(std::move(tag));
    SendOKEmpty(ctx);
}

void OnTagUpdate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    CString name, triggerId;
    int persistence = -1;
    ParamExtractor ex;
    ex.Add("id",          [&](std::string_view v) { id        = CString(v.data(), v.size()); });
    ex.Add("name",        [&](std::string_view v) { name      = CString(v.data(), v.size()); });
    ex.Add("triggerId",   [&](std::string_view v) { triggerId = CString(v.data(), v.size()); });
    ex.Add("persistence", [&](std::string_view v) { persistence = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);
    auto* tag = DB::Tags.TryLookup(id);
    if (!tag) {
        SendError(ctx, CBError::TagNotFound);
        return;
    }

    if (!name.IsEmpty()) {
        tag->name = name;
    }
    if (!triggerId.IsEmpty()) {
        tag->triggerId = triggerId;
    }
    if (persistence >= 0) {
        tag->persistence = persistence;
    }
    SendOKEmpty(ctx);
}

void OnTagDelete(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    if (!DB::Tags.DeleteByID(id)) {
        SendError(ctx, CBError::TagNotFound);
    } else {
        SendOKEmpty(ctx);
    }
}
