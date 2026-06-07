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

/* ── Query callbacks (no input params) ─────────────────────────────────── */

void OnTriggerList(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto& db = DB::Triggers;
    auto count = db.Size();
    auto items = std::make_unique<RPCB_StrView[]>(count);
    RPCB_StrViewList list = {items.get(), 0};
    for (size_t i = 0; i < count; ++i) {
        auto const& t = db.Nth(i);
        list.items[list.count].data = t.ID();
        list.items[list.count].len  = t.ID().GetLength();
        list.count++;
    }
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, EmptyStrView(), &list);
}

void OnTriggerGetBasic(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }
    auto& opts = t->Options();
    RPCB_StrView items[16];
    size_t n = 0;
    SetKeyValue(items, n, "id",          t->ID());
    SetKeyValue(items, n, "name",        opts.Name());
    SetKeyValue(items, n, "house",       opts.house);
    SetKeyValue(items, n, "nextTrigger", opts.nextTrigger);
    WriteControls(items, n, opts);
    SendOK(ctx, items, n);
}

void OnTriggerEventCount(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }
    CString cnt;
    cnt.Format("%zu", t->Events().Size());
    RPCB_StrView items[2];
    size_t n = 0;
    SetKeyValue(items, n, "count", cnt);
    SendOK(ctx, items, n);
}

void OnTriggerActionCount(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }
    CString cnt;
    cnt.Format("%zu", t->Actions().Size());
    RPCB_StrView items[2];
    size_t n = 0;
    SetKeyValue(items, n, "count", cnt);
    SendOK(ctx, items, n);
}

/* ── Callbacks with params — use ParamExtractor ────────────────────────── */

void OnTriggerEventGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    int idx = 0;
    ParamExtractor ex;
    ex.Add("index", [&](std::string_view v) { idx = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);

    if (idx < 0 || static_cast<size_t>(idx) >= t->Events().Size()) {
        SendError(ctx, CBError::EventIndexInvalid);
        return;
    }
    RPCB_StrView items[6];
    size_t n = 0;
    WriteEvent(items, n, t->Events().Nth(idx));
    SendOK(ctx, items, n);
}

void OnTriggerActionGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    int idx = 0;
    ParamExtractor ex;
    ex.Add("index", [&](std::string_view v) { idx = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);

    if (idx < 0 || static_cast<size_t>(idx) >= t->Actions().Size()) {
        SendError(ctx, CBError::ActionIndexInvalid);
        return;
    }
    RPCB_StrView items[14];
    size_t n = 0;
    WriteAction(items, n, t->Actions().Nth(idx));
    SendOK(ctx, items, n);
}

void OnTriggerCreate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString newId, name, house;
    name = "New Trigger";
    ParamExtractor ex;
    ex.Add("id",    [&](std::string_view v) { newId = CString(v.data(), v.size()); });
    ex.Add("name",  [&](std::string_view v) { name  = CString(v.data(), v.size()); });
    ex.Add("house", [&](std::string_view v) { house = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);

    if (newId.IsEmpty()) {
        SendError(ctx, CBError::MissingFieldId);
        return;
    }
    if (DB::Triggers.Exists(newId)) {
        SendError(ctx, CBError::TriggerAlreadyExists);
        return;
    }
    DB::Triggers.Append(TriggerInstance(std::move(newId), std::move(name), std::move(house)));
    SendOKEmpty(ctx);
}

void OnTriggerUpdate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto* t = DB::Triggers.TryLookup(IdFromContext(ctx));
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    CString name, house;
    ParamExtractor ex;
    ex.Add("name",  [&](std::string_view v) { name  = CString(v.data(), v.size()); });
    ex.Add("house", [&](std::string_view v) { house = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);

    if (!name.IsEmpty()) {
        t->SetName(name, true);
    }
    if (!house.IsEmpty()) {
        t->Options().house = house;
    }
    SendOKEmpty(ctx);
}

void OnTriggerDelete(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    if (!DB::Triggers.DeleteByID(IdFromContext(ctx))) {
        SendError(ctx, CBError::TriggerNotFound);
    } else {
        SendOKEmpty(ctx);
    }
}

void OnUpdateDialogs(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    theApp.MainWindow()->UpdateDialogs();
    SendOKEmpty(ctx);
}
