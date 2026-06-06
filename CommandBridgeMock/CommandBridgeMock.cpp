#include "3rdParty/CommandBridge/CommandBridge.h"

#include <unordered_map>
#include <string>
#include <atomic>

struct CallbackEntry {
    RPCB_ActionCallback callback;
    void*               userData;
};

static std::unordered_map<std::string, CallbackEntry> s_actions;
static std::atomic<bool> s_initialized{false};

/* ── Standard C API ─────────────────────────────────────────────────────── */

extern "C" {

COMMAND_BRIDGE_EXPORT int32_t RPCB_Init(const CommandBridgeInitArgs* args) {
    if (s_initialized.exchange(true)) {
        return RPCB_ERR_ALREADY_INIT;
    }
    if (!args || args->reserved != 0) {
        s_initialized = false;
        return RPCB_ERR_INVALID_ARG;
    }
    for (size_t i = 0; i < args->actionCount; ++i) {
        auto const& def = args->actions[i];
        s_actions.emplace(std::string(def.name),
                          CallbackEntry{def.callback, def.userData});
    }
    return RPCB_SUCCESS;
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_Shutdown(void) {
    if (!s_initialized.exchange(false)) {
        return RPCB_ERR_NOT_INIT;
    }
    s_actions.clear();
    return RPCB_SUCCESS;
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_IsRunning(int32_t* outPort) {
    if (!s_initialized.load()) {
        return 0;
    }
    if (outPort) {
        *outPort = 4333;
    }
    return 1;
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_RegisterAction(const char* actionName,
                                                   RPCB_ActionCallback callback,
                                                   void* userData) {
    if (!actionName || !callback) {
        return RPCB_ERR_INVALID_ARG;
    }
    if (!s_initialized.load()) {
        return RPCB_ERR_NOT_INIT;
    }
    s_actions.emplace(std::string(actionName), CallbackEntry{callback, userData});
    return RPCB_SUCCESS;
}

/* ── Test-only functions ─────────────────────────────────────────────────── */

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestDispatch(const char* actionName,
                                                 RPCB_CallContext* ctx) {
    if (!actionName || !ctx) {
        return RPCB_ERR_INVALID_ARG;
    }
    auto it = s_actions.find(actionName);
    if (it == s_actions.end()) {
        ctx->statusCode = 404;
        return RPCB_SUCCESS;
    }
    ctx->userData = it->second.userData;
    ctx->statusCode = 200;
    it->second.callback(ctx);
    return RPCB_SUCCESS;
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetActionCount(void) {
    return static_cast<int32_t>(s_actions.size());
}

} // extern "C"
