#include "StdAfx.h"
#include "CommandBridgeClient.h"

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
        {"update_dialogs",       OnUpdateDialogs,       nullptr},
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
