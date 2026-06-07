#include "stdafx.h"
#include "CommandBridgeClient.h"
#include "TriggerDatabase.h"
#include "TriggerDef.h"
#include <memory>
#include <string>

/* ── Resolve macro ─────────────────────────────────────────────────────── */

#define ASSIGN_IMPORT_FUNC(dll, s, func)                          \
    s.p##func = reinterpret_cast<decltype(s.p##func)>(            \
        GetProcAddress(dll, #func))

/* ── Proxy implementation ──────────────────────────────────────────────── */

CommandBridgeProxy::~CommandBridgeProxy() {
    Unload();
}

bool CommandBridgeProxy::Load(const char* dllName) {
    HMODULE hMod = ::LoadLibraryA(dllName);
    if (!hMod) {
        return false;
    }

    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_Init);
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_Shutdown);
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_IsRunning);
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_RegisterAction);
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_SendResponse);

    if (!m_imports.pRPCB_Init || !m_imports.pRPCB_Shutdown) {
        ::FreeLibrary(hMod);
        return false;
    }

    m_hModule = hMod;
    return true;
}

void CommandBridgeProxy::Unload() {
    if (m_hModule) {
        ::FreeLibrary(static_cast<HMODULE>(m_hModule));
        m_hModule = nullptr;
    }
    m_imports = {};
}

bool CommandBridgeProxy::IsLoaded() const {
    return m_hModule != nullptr;
}

namespace {

/* ── Output helpers ────────────────────────────────────────────────────── */

void SetKeyValue(RPCB_StrView* items, size_t& n, const char* k, const CString& v) {
    items[n].data = k;
    items[n].len  = strlen(k);
    n++;
    items[n].data = v;
    items[n].len  = v.GetLength();
    n++;
}

void SendKV(RPCB_CallContext* ctx, RPCB_StrView* items, size_t n, int32_t code) {
    RPCB_StrViewList list = {items, n};
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, code, &list);
}

void SendEmpty(RPCB_CallContext* ctx, int32_t code) {
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, code, nullptr);
}

/* ── Param lookup helper ───────────────────────────────────────────────── */

CString FindParam(const RPCB_StrViewList& params, const char* key) {
    for (size_t i = 0; i + 1 < params.count; i += 2) {
        auto const& k = params.items[i];
        if (k.len == strlen(key) && strncmp(k.data, key, k.len) == 0) {
            return CString(params.items[i + 1].data, params.items[i + 1].len);
        }
    }
    return {};
}

/* ── Trigger callbacks ─────────────────────────────────────────────────── */

void OnTriggerList(RPCB_CallContext* ctx) {
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
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, 200, &list);
}

void OnTriggerGetBasic(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    if (!t) {
        SendEmpty(ctx, 404);
        return;
    }
    auto& opts = t->Options();
    RPCB_StrView items[16];
    size_t n = 0;

    SetKeyValue(items, n, "id",          t->ID());
    SetKeyValue(items, n, "name",        opts.Name());
    SetKeyValue(items, n, "house",       opts.house);
    SetKeyValue(items, n, "nextTrigger", opts.nextTrigger);

    CString s;
    s.Format("%d", opts.controls[TriggerOptions::Disable] ? 1 : 0);
    SetKeyValue(items, n, "disabled", s);
    s.Format("%d", opts.controls[TriggerOptions::Easy] ? 1 : 0);
    SetKeyValue(items, n, "easy", s);
    s.Format("%d", opts.controls[TriggerOptions::Medium] ? 1 : 0);
    SetKeyValue(items, n, "medium", s);
    s.Format("%d", opts.controls[TriggerOptions::Hard] ? 1 : 0);
    SetKeyValue(items, n, "hard", s);
    s.Format("%d", opts.controls[TriggerOptions::MustTransfer] ? 1 : 0);
    SetKeyValue(items, n, "mustTransfer", s);

    SendKV(ctx, items, n, 200);
}

void OnTriggerEventCount(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    CString cnt;
    cnt.Format("%zu", t ? t->Events().Size() : 0);
    RPCB_StrView items[2];
    size_t n = 0;
    SetKeyValue(items, n, "count", cnt);
    SendKV(ctx, items, n, t ? 200 : 404);
}

void OnTriggerEventGet(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    if (!t) {
        SendEmpty(ctx, 404);
        return;
    }
    int idx = std::atoi(FindParam(ctx->params, "index"));
    if (idx < 0 || static_cast<size_t>(idx) >= t->Events().Size()) {
        SendEmpty(ctx, 404);
        return;
    }
    auto& ev = t->Events().Nth(idx);
    RPCB_StrView items[6];
    size_t n = 0;

    CString s;
    s.Format("%d", ev.eventType);
    SetKeyValue(items, n, "eventType", s);
    SetKeyValue(items, n, "param1", ev.param1);
    if (ev.param2) {
        SetKeyValue(items, n, "param2", *ev.param2);
    }
    SendKV(ctx, items, n, 200);
}

void OnTriggerActionCount(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    CString cnt;
    cnt.Format("%zu", t ? t->Actions().Size() : 0);
    RPCB_StrView items[2];
    size_t n = 0;
    SetKeyValue(items, n, "count", cnt);
    SendKV(ctx, items, n, t ? 200 : 404);
}

void OnTriggerActionGet(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    if (!t) {
        SendEmpty(ctx, 404);
        return;
    }
    int idx = std::atoi(FindParam(ctx->params, "index"));
    if (idx < 0 || static_cast<size_t>(idx) >= t->Actions().Size()) {
        SendEmpty(ctx, 404);
        return;
    }
    auto& act = t->Actions().Nth(idx);
    RPCB_StrView items[14];
    size_t n = 0;

    CString s;
    s.Format("%d", act.ActionType());
    SetKeyValue(items, n, "actionType", s);
    s.Format("%d", act.ActionCode());
    SetKeyValue(items, n, "actionCode", s);
    s.Format("%d", act.Waypoint());
    SetKeyValue(items, n, "waypoint", s);

    char key[8];
    for (int pi = 0; pi < 5; ++pi) {
        sprintf_s(key, "param%d", pi);
        SetKeyValue(items, n, key, act.Params()[pi]);
    }
    SendKV(ctx, items, n, 200);
}

void OnTriggerCreate(RPCB_CallContext* ctx) {
    CString newId     = FindParam(ctx->params, "id");
    CString name      = FindParam(ctx->params, "name");
    CString house     = FindParam(ctx->params, "house");
    if (name.IsEmpty()) {
        name = "New Trigger";
    }
    if (newId.IsEmpty()) {
        SendEmpty(ctx, 400);
        return;
    }
    if (DB::Triggers.Exists(newId)) {
        SendEmpty(ctx, 409);
        return;
    }
    DB::Triggers.Append(TriggerInstance(std::move(newId), std::move(name), std::move(house)));
    SendEmpty(ctx, 201);
}

void OnTriggerUpdate(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* t = DB::Triggers.TryLookup(triggerId);
    if (!t) {
        SendEmpty(ctx, 404);
        return;
    }
    CString name  = FindParam(ctx->params, "name");
    CString house = FindParam(ctx->params, "house");
    if (!name.IsEmpty()) {
        t->SetName(name, true);
    }
    if (!house.IsEmpty()) {
        t->Options().house = house;
    }
    SendEmpty(ctx, 200);
}

void OnTriggerDelete(RPCB_CallContext* ctx) {
    CString triggerId(ctx->uniqueId.data, ctx->uniqueId.len);
    int32_t code = DB::Triggers.DeleteByID(triggerId) ? 200 : 404;
    SendEmpty(ctx, code);
}

/* ── Tag callbacks ─────────────────────────────────────────────────────── */

void OnTagList(RPCB_CallContext* ctx) {
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
    CommandBridgeClient::Instance().SendResponse(ctx->uniqueId, 200, &list);
}

void OnTagGet(RPCB_CallContext* ctx) {
    CString tagId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* tag = DB::Tags.TryLookup(tagId);
    if (!tag) {
        SendEmpty(ctx, 404);
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
    SendKV(ctx, items, n, 200);
}

void OnTagCreate(RPCB_CallContext* ctx) {
    CString newId       = FindParam(ctx->params, "id");
    CString name        = FindParam(ctx->params, "name");
    CString triggerId   = FindParam(ctx->params, "triggerId");
    CString persistStr  = FindParam(ctx->params, "persistence");
    if (name.IsEmpty()) {
        name = "New Tag";
    }
    if (newId.IsEmpty()) {
        SendEmpty(ctx, 400);
        return;
    }
    if (DB::Tags.Exists(newId)) {
        SendEmpty(ctx, 409);
        return;
    }
    TagInstance tag(newId);
    tag.name        = name;
    tag.triggerId   = triggerId;
    tag.persistence = std::atoi(persistStr);
    DB::Tags.Append(std::move(tag));
    SendEmpty(ctx, 201);
}

void OnTagUpdate(RPCB_CallContext* ctx) {
    CString tagId(ctx->uniqueId.data, ctx->uniqueId.len);
    auto* tag = DB::Tags.TryLookup(tagId);
    if (!tag) {
        SendEmpty(ctx, 404);
        return;
    }
    CString name      = FindParam(ctx->params, "name");
    CString triggerId = FindParam(ctx->params, "triggerId");
    CString persist   = FindParam(ctx->params, "persistence");
    if (!name.IsEmpty()) {
        tag->name = name;
    }
    if (!triggerId.IsEmpty()) {
        tag->triggerId = triggerId;
    }
    if (!persist.IsEmpty()) {
        tag->persistence = std::atoi(persist);
    }
    SendEmpty(ctx, 200);
}

void OnTagDelete(RPCB_CallContext* ctx) {
    CString tagId(ctx->uniqueId.data, ctx->uniqueId.len);
    int32_t code = DB::Tags.DeleteByID(tagId) ? 200 : 404;
    SendEmpty(ctx, code);
}

} // namespace

/* ── CommandBridgeClient singleton ─────────────────────────────────────── */

CommandBridgeClient& CommandBridgeClient::Instance() {
    static CommandBridgeClient client;
    return client;
}

bool CommandBridgeClient::Init() {
    if (m_initialized) {
        return true;
    }

    if (!m_proxy.Load()) {
        TRACE("CommandBridge.dll not loaded; RPC bridge unavailable.\n");
        return false;
    }

    CommandBridgeActionDef actions[] = {
        {"trigger_list",         OnTriggerList,         nullptr},
        {"trigger_get_basic",    OnTriggerGetBasic,     nullptr},
        {"trigger_event_count",  OnTriggerEventCount,   nullptr},
        {"trigger_event_get",    OnTriggerEventGet,     nullptr},
        {"trigger_action_count", OnTriggerActionCount,  nullptr},
        {"trigger_action_get",   OnTriggerActionGet,    nullptr},
        {"trigger_create",       OnTriggerCreate,       nullptr},
        {"trigger_update",       OnTriggerUpdate,       nullptr},
        {"trigger_delete",       OnTriggerDelete,       nullptr},
        {"tag_list",             OnTagList,             nullptr},
        {"tag_get",              OnTagGet,              nullptr},
        {"tag_create",           OnTagCreate,           nullptr},
        {"tag_update",           OnTagUpdate,           nullptr},
        {"tag_delete",           OnTagDelete,           nullptr},
    };

    CommandBridgeInitArgs args = {};
    args.actions     = actions;
    args.actionCount = std::size(actions);

    m_initialized = (m_proxy.Init(&args) == RPCB_SUCCESS);
    return m_initialized;
}

void CommandBridgeClient::Shutdown() {
    if (m_proxy.IsLoaded()) {
        m_proxy.Shutdown();
    }
    m_initialized = false;
}
