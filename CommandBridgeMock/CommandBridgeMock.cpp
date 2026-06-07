#include "CommandBridge.h"

#include <unordered_map>
#include <string>
#include <atomic>

/* Test-only exports — not in the public CommandBridge.h */
extern "C" {
    COMMAND_BRIDGE_EXPORT int32_t RPCB_TestDispatch(const char*, RPCB_CallContext*);
    COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetActionCount(void);
    COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetResponse(const char*, int32_t*, char*, size_t);
}

struct CallbackEntry {
    RPCB_ActionCallback callback;
    void*               userData;
};

struct StoredResponse {
    std::string body;
    int32_t     statusCode;
};

static std::unordered_map<std::string, CallbackEntry> s_actions;
static std::unordered_map<std::string, StoredResponse> s_responses;
static std::atomic<bool> s_initialized{false};

/* ── Standard C API ─────────────────────────────────────────────────────── */

extern "C" {

COMMAND_BRIDGE_EXPORT int32_t RPCB_Init(const CommandBridgeInitArgs* args) {
    if (s_initialized.exchange(true)) {
        return RPCB_ERR_ALREADY_INIT;
    }
    if (!args) {
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
    s_responses.clear();
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

COMMAND_BRIDGE_EXPORT int32_t RPCB_SendResponse(RPCB_StrView uniqueId,
                                                 const RPCB_Response* response) {
    std::string uid(uniqueId.data, uniqueId.len);
    StoredResponse resp;
    if (!response || response->errorDetail.len > 0) {
        resp.statusCode = 500;
    } else {
        resp.statusCode = 200;
    }
    if (response && response->items.count > 0) {
        std::string body;
        if (response->paramType == RPCB_PARAM_STRING_ARRAY) {
            for (size_t i = 0; i < response->items.count; ++i) {
                if (i > 0) { body += ","; }
                body += "\"" + std::string(response->items.items[i].data,
                                           response->items.items[i].len) + "\"";
            }
            body = "[" + body + "]";
        } else {
            for (size_t i = 0; i + 1 < response->items.count; i += 2) {
                if (!body.empty()) { body += ","; }
                body += "\"" + std::string(response->items.items[i].data,
                                           response->items.items[i].len) + "\":";
                body += "\"" + std::string(response->items.items[i+1].data,
                                           response->items.items[i+1].len) + "\"";
            }
            body = "{" + body + "}";
        }
        resp.body = std::move(body);
    }
    s_responses.emplace(std::move(uid), std::move(resp));
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
        return RPCB_ERR_NOT_FOUND;
    }

    std::string uid(ctx->uniqueId.data, ctx->uniqueId.len);
    s_responses.erase(uid); // clear any previous response

    ctx->userData = it->second.userData;
    it->second.callback(ctx);

    // Check if callback sent a response
    auto rit = s_responses.find(uid);
    if (rit != s_responses.end()) {
        ctx->userData = reinterpret_cast<void*>(static_cast<intptr_t>(rit->second.statusCode));
        return RPCB_SUCCESS;
    }
    return RPCB_ERR_NOT_FOUND;
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetActionCount(void) {
    return static_cast<int32_t>(s_actions.size());
}

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetResponse(const char* uniqueId,
                                                    int32_t* outStatusCode,
                                                    char* outBody, size_t bodySize) {
    std::string uid(uniqueId);
    auto it = s_responses.find(uid);
    if (it == s_responses.end()) {
        return RPCB_ERR_NOT_FOUND;
    }
    if (outStatusCode) {
        *outStatusCode = it->second.statusCode;
    }
    if (outBody && bodySize > 0) {
        size_t copyLen = it->second.body.size() < bodySize - 1
            ? it->second.body.size() : bodySize - 1;
        memcpy(outBody, it->second.body.data(), copyLen);
        outBody[copyLen] = '\0';
    }
    return RPCB_SUCCESS;
}

} // extern "C"
