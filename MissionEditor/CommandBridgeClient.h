#pragma once

#include "CommandBridge.h"

/* ── Error constants ─────────────────────────────────────────────────────── */

namespace CBError {
    inline constexpr RPCB_StrView MapNotLoaded        = ToStrView("map not loaded");
    inline constexpr RPCB_StrView TriggerNotFound     = ToStrView("trigger not found");
    inline constexpr RPCB_StrView TagNotFound         = ToStrView("tag not found");
    inline constexpr RPCB_StrView EventIndexInvalid   = ToStrView("event index invalid");
    inline constexpr RPCB_StrView ActionIndexInvalid  = ToStrView("action index invalid");
    inline constexpr RPCB_StrView MissingFieldId      = ToStrView("missing field: id");
    inline constexpr RPCB_StrView TriggerAlreadyExists = ToStrView("trigger already exists");
    inline constexpr RPCB_StrView TagAlreadyExists    = ToStrView("tag already exists");
}

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
    int32_t SendResponse(RPCB_StrView uniqueId, RPCB_StrView errorDetail,
                         const RPCB_StrViewList* outputData) {
        return CALL_IMPORT_FUNC(RPCB_SendResponse, RPCB_ERR_NOT_INIT, uniqueId, errorDetail, outputData);
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

    int32_t SendResponse(RPCB_StrView uniqueId, RPCB_StrView errorDetail,
                         const RPCB_StrViewList* outputData) {
        return m_proxy.SendResponse(uniqueId, errorDetail, outputData);
    }

private:
    CommandBridgeClient() = default;
    CommandBridgeProxy m_proxy;
    bool m_initialized = false;
};

/* ── Callback declarations ────────────────────────────────────────────── */

extern void OnTriggerList(RPCB_CallContext* ctx);
extern void OnTriggerGetBasic(RPCB_CallContext* ctx);
extern void OnTriggerEventCount(RPCB_CallContext* ctx);
extern void OnTriggerEventGet(RPCB_CallContext* ctx);
extern void OnTriggerActionCount(RPCB_CallContext* ctx);
extern void OnTriggerActionGet(RPCB_CallContext* ctx);
extern void OnTriggerCreate(RPCB_CallContext* ctx);
extern void OnTriggerUpdate(RPCB_CallContext* ctx);
extern void OnTriggerDelete(RPCB_CallContext* ctx);

extern void OnTagList(RPCB_CallContext* ctx);
extern void OnTagGet(RPCB_CallContext* ctx);
extern void OnTagCreate(RPCB_CallContext* ctx);
extern void OnTagUpdate(RPCB_CallContext* ctx);
extern void OnTagDelete(RPCB_CallContext* ctx);

extern void OnUpdateDialogs(RPCB_CallContext* ctx);
