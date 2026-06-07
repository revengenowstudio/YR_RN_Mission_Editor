#include "StdAfx.h"
#include "CommandBridgeClient.h"
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

/* ── Query callbacks (no input params) ─────────────────────────────────── */

void OnTriggerList(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    auto& db = DB::Triggers;
    auto count = db.Size();
    auto items = std::make_unique<RPCB_StrView[]>(count);
    size_t n = 0;
    for (size_t i = 0; i < count; ++i) {
        items[n].data = db.Nth(i).ID();
        items[n].len = db.Nth(i).ID().GetLength();
        n++;
    }
    SendArrayOK(ctx, items.get(), n);
}

void OnTriggerGetBasic(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }
    auto& opts = t->Options();
    TriggerCacheBuilder items;
    items.PushKeyValue("id", t->ID());
    items.PushKeyValue("name", opts.Name());
    items.PushKeyValue("house", opts.house);
    items.PushKeyValue("nextTrigger", opts.nextTrigger);
    items.WriteControls(opts);

    SendOK(ctx, items.Build());
}

void OnTriggerEventCount(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }
    CacheBuilder items;
    items.PushKeyValue("count", t->Events().Size());
    SendOK(ctx, items.Build());
}

void OnTriggerActionCount(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    CacheBuilder items;
    items.PushKeyValue("count", t->Actions().Size());
    SendOK(ctx, items.Build());
}

/* ── Callbacks with params — use ParamExtractor ────────────────────────── */

void OnTriggerEventGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    int idx = 0;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Add("index", [&](std::string_view v) { idx = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    if (idx < 0 || static_cast<size_t>(idx) >= t->Events().Size()) {
        SendError(ctx, CBError::EventIndexInvalid);
        return;
    }
    TriggerCacheBuilder items;
    items.WriteEvent(t->Events().Nth(idx));
    SendOK(ctx, items.Build());
}

void OnTriggerActionGet(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id;
    int idx = 0;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Add("index", [&](std::string_view v) { idx = std::atoi(std::string(v).c_str()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

    if (idx < 0 || static_cast<size_t>(idx) >= t->Actions().Size()) {
        SendError(ctx, CBError::ActionIndexInvalid);
        return;
    }
    TriggerCacheBuilder items;
    items.WriteAction(t->Actions().Nth(idx));
    SendOK(ctx, items.Build());
}

void OnTriggerCreate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString name, house;
    name = "New Trigger";
    ParamExtractor ex;
    ex.Add("name", [&](std::string_view v) { name = CString(v.data(), v.size()); });
    ex.Add("house", [&](std::string_view v) { house = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);

    DB::Triggers.Append(TriggerInstance(GetFreeID(), std::move(name), std::move(house)));
    SendOKEmpty(ctx);
}

void OnTriggerUpdate(RPCB_CallContext* ctx) {
    if (!IsMapReady()) {
        SendError(ctx, CBError::MapNotLoaded);
        return;
    }
    CString id, name, house;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Add("name", [&](std::string_view v) { name = CString(v.data(), v.size()); });
    ex.Add("house", [&](std::string_view v) { house = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    auto* t = DB::Triggers.TryLookup(id);
    if (!t) {
        SendError(ctx, CBError::TriggerNotFound);
        return;
    }

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
    CString id;
    ParamExtractor ex;
    ex.Add("id", [&](std::string_view v) { id = CString(v.data(), v.size()); });
    ex.Extract(ctx->params);
    if (!DB::Triggers.DeleteByID(id)) {
        SendError(ctx, CBError::TriggerNotFound);
    }
    else {
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
