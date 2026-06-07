#pragma once

#include "CommandBridge.h"

/* ── Import helper macros ───────────────────────────────────────────────── */

#define DEFINE_IMPORT_FUNC(func) decltype(&func) p##func

#define CALL_IMPORT_FUNC(func, defaultRet, ...) \
    (m_imports.p##func ? m_imports.p##func(__VA_ARGS__) : (defaultRet))

/* ── Imported function table ────────────────────────────────────────────── */

struct CommandBridgeImports {
    DEFINE_IMPORT_FUNC(RPCB_Init);
    DEFINE_IMPORT_FUNC(RPCB_Shutdown);
    DEFINE_IMPORT_FUNC(RPCB_IsRunning);
    DEFINE_IMPORT_FUNC(RPCB_RegisterAction);
    DEFINE_IMPORT_FUNC(RPCB_SendResponse);
};

/* ── Proxy class ────────────────────────────────────────────────────────── */

class CommandBridgeProxy {
public:
    CommandBridgeProxy() = default;
    ~CommandBridgeProxy();

    CommandBridgeProxy(const CommandBridgeProxy&) = delete;
    CommandBridgeProxy& operator=(const CommandBridgeProxy&) = delete;

    bool Load(const char* dllName = "CommandBridge.dll");
    void Unload();
    bool IsLoaded() const;

    int32_t Init(const CommandBridgeInitArgs* args) {
        return CALL_IMPORT_FUNC(RPCB_Init, RPCB_ERR_NOT_INIT, args);
    }
    int32_t Shutdown() {
        return CALL_IMPORT_FUNC(RPCB_Shutdown, RPCB_ERR_NOT_INIT);
    }
    int32_t IsRunning(int32_t* outPort) {
        return CALL_IMPORT_FUNC(RPCB_IsRunning, RPCB_ERR_NOT_INIT, outPort);
    }
    int32_t RegisterAction(const char* name, RPCB_ActionCallback cb, void* data) {
        return CALL_IMPORT_FUNC(RPCB_RegisterAction, RPCB_ERR_NOT_INIT, name, cb, data);
    }
    int32_t SendResponse(RPCB_StrView uniqueId, int32_t statusCode,
                         const RPCB_StrViewList* outputData) {
        return CALL_IMPORT_FUNC(RPCB_SendResponse, RPCB_ERR_NOT_INIT, uniqueId, statusCode, outputData);
    }

protected:
    void*                m_hModule = nullptr;  // HMODULE, cast in .cpp
    CommandBridgeImports m_imports = {};
};

/* ── Editor integration singleton ─────────────────────────────────────── */

class CommandBridgeClient {
public:
    static CommandBridgeClient& Instance();

    bool Init();
    void Shutdown();
    bool IsInitialized() const { return m_initialized; }

    int32_t SendResponse(RPCB_StrView uniqueId, int32_t statusCode,
                         const RPCB_StrViewList* outputData) {
        return m_proxy.SendResponse(uniqueId, statusCode, outputData);
    }

private:
    CommandBridgeClient() = default;
    CommandBridgeProxy m_proxy;
    bool m_initialized = false;
};
