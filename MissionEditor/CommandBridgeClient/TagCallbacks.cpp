#include "StdAfx.h"
#include "CommandBridgeClient.h"
#include "Serde.h"
#include "Handler.h"
#include "TriggerDatabase.h"
#include "variables.h"

namespace {

using namespace Serde;

bool IsMapReady() {
    return Map != nullptr;
}

void SendError(RPCB_CallContext* ctx, RPCB_StrView err) {
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, err, nullptr);
}

void SendOK(RPCB_CallContext* ctx, RPCB_StrView* items, size_t n) {
    RPCB_StrViewList list = {items, n};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, EmptyStrView(), &list);
}

void SendOKEmpty(RPCB_CallContext* ctx) {
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, EmptyStrView(), nullptr);
}

CString IdFromContext(RPCB_CallContext* ctx) {
    return CString(ctx->uniqueId.data, ctx->uniqueId.len);
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
    RPCB_StrViewList list = {items.get(), 0};
    for (size_t i = 0; i < count; ++i) {
        auto const& tag = db.Nth(i);
        list.items[list.count].data = tag.id;
        list.items[list.count].len  = tag.id.GetLength();
        list.count++;
    }
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, EmptyStrView(), &list);
}

void OnTagGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* tag = DB::Tags.TryLookup(IdFromContext(ctx));
    if (!tag) {
        SendError(ctx, CBError::TagNotFound);
        return;
    }
    RPCB_StrView items[8];
    size_t n = 0;
    SetKeyValue(items, n, "id",          tag->id);
    SetKeyValue(items, n, "name",        tag->name);
    SetKeyValue(items, n, "triggerId",   tag->triggerId);
    CString s;
    s.Format("%d", tag->persistence);
    SetKeyValue(items, n, "persistence", s);
    SendOK(ctx, items, n);
}

/* ── Callbacks with params — use ParamExtractor ────────────────────────── */

void OnTagCreate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString newId, name, triggerId;
    int persistence = 0;
    name = "New Tag";
    ParamExtractor ex;
    ex.Add("id",          [&](std::string_view v) { newId     = CString(v.data(), v.size()); });
    ex.Add("name",        [&](std::string_view v) { name      = CString(v.data(), v.size()); });
    ex.Add("triggerId",   [&](std::string_view v) { triggerId = CString(v.data(), v.size()); });
    ex.Add("persistence", [&](std::string_view v) { persistence = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);

    if (newId.IsEmpty()) {
        SendError(ctx, CBError::MissingFieldId);
        return;
    }
    if (DB::Tags.Exists(newId)) {
        SendError(ctx, CBError::TagAlreadyExists);
        return;
    }
    TagInstance tag(newId);
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
    auto* tag = DB::Tags.TryLookup(IdFromContext(ctx));
    if (!tag) {
        SendError(ctx, CBError::TagNotFound);
        return;
    }

    CString name, triggerId;
    int persistence = -1;
    ParamExtractor ex;
    ex.Add("name",        [&](std::string_view v) { name      = CString(v.data(), v.size()); });
    ex.Add("triggerId",   [&](std::string_view v) { triggerId = CString(v.data(), v.size()); });
    ex.Add("persistence", [&](std::string_view v) { persistence = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);

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
    if (!DB::Tags.DeleteByID(IdFromContext(ctx))) {
        SendError(ctx, CBError::TagNotFound);
    } else {
        SendOKEmpty(ctx);
    }
}
