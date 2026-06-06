#include "stdafx.h"
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
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_TestDispatch);
    ASSIGN_IMPORT_FUNC(hMod, m_imports, RPCB_TestGetActionCount);

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
