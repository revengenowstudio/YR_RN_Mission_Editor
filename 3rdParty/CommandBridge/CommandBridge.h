#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
   Export / Import macro
   -------------------------------------------------------------------------- */

#if defined(_WIN32) || defined(_WIN64)
  #ifdef COMMAND_BRIDGE_EXPORTS
    #define COMMAND_BRIDGE_EXPORT __declspec(dllexport)
  #else
    #define COMMAND_BRIDGE_EXPORT __declspec(dllimport)
  #endif
#elif defined(__linux__) || defined(__APPLE__)
  #define COMMAND_BRIDGE_EXPORT __attribute__((visibility("default")))
#else
  #define COMMAND_BRIDGE_EXPORT
#endif

/* --------------------------------------------------------------------------
   String view types
   -------------------------------------------------------------------------- */

typedef struct RPCB_StrView {
    const char* data;
    size_t      len;
} RPCB_StrView;

typedef struct RPCB_StrViewList {
    const RPCB_StrView* items;
    size_t        count;
} RPCB_StrViewList;

/* --------------------------------------------------------------------------
   Parameter type enum
   -------------------------------------------------------------------------- */

typedef enum RPCB_ParamType {
    RPCB_PARAM_STRING_LIST  = 0,  /* key-value pairs → JSON object */
    RPCB_PARAM_STRING_ARRAY = 1,  /* value-only list  → JSON array  */
} RPCB_ParamType;

/* --------------------------------------------------------------------------
   Call context & callback signature
   -------------------------------------------------------------------------- */

typedef struct RPCB_CallContext {
    RPCB_StrView    uniqueId;   /* bridge-generated transaction ID — pass back verbatim to RPCB_SendResponse */
    RPCB_ParamType  paramType;
    RPCB_StrViewList params;    /* input params (valid during callback) */
    void*           userData;
} RPCB_CallContext;

typedef void (*RPCB_ActionCallback)(RPCB_CallContext* ctx);

/* --------------------------------------------------------------------------
   Response descriptor
   -------------------------------------------------------------------------- */

typedef struct RPCB_Response {
    RPCB_ParamType  paramType;    /* STRING_LIST (object) or STRING_ARRAY (array) */
    RPCB_StrView    errorDetail;  /* empty = success; non-empty = error description */
    RPCB_StrViewList items;        /* output data */
} RPCB_Response;

/* --------------------------------------------------------------------------
   Response function — called by the editor INSIDE a callback.
   The bridge copies all data before returning.
   -------------------------------------------------------------------------- */

COMMAND_BRIDGE_EXPORT int32_t RPCB_SendResponse(
    RPCB_StrView            uniqueId,
    const RPCB_Response*    response
);

/* --------------------------------------------------------------------------
   Init args
   -------------------------------------------------------------------------- */

typedef struct CommandBridgeActionDef {
    const char*          name;
    RPCB_ActionCallback  callback;
    void*                userData;
} CommandBridgeActionDef;

typedef struct CommandBridgeInitArgs {
    const CommandBridgeActionDef* actions;
    size_t                        actionCount;
} CommandBridgeInitArgs;

/* --------------------------------------------------------------------------
   Error codes
   -------------------------------------------------------------------------- */

#define RPCB_SUCCESS           0
#define RPCB_ERR_NOT_INIT     -1
#define RPCB_ERR_ALREADY_INIT -2
#define RPCB_ERR_INVALID_ARG  -3
#define RPCB_ERR_SERVER_FAIL  -4
#define RPCB_ERR_NOT_FOUND    -5

/* --------------------------------------------------------------------------
   Exported functions
   -------------------------------------------------------------------------- */

COMMAND_BRIDGE_EXPORT int32_t RPCB_Init(const CommandBridgeInitArgs* args);

COMMAND_BRIDGE_EXPORT int32_t RPCB_Shutdown(void);

COMMAND_BRIDGE_EXPORT int32_t RPCB_IsRunning(int32_t* outPort);

COMMAND_BRIDGE_EXPORT int32_t RPCB_RegisterAction(
    const char*          actionName,
    RPCB_ActionCallback  callback,
    void*                userData
);

/* (Deferred — not implemented yet.)
   COMMAND_BRIDGE_EXPORT int32_t RPCB_UnregisterAction(const char* actionName); */

#ifdef __cplusplus
}

/* Convert a string literal to RPCB_StrView (excludes null terminator). */
template <size_t N>
constexpr RPCB_StrView ToStrView(const char (&s)[N]) {
    return {s, N - 1};
}

inline RPCB_StrView EmptyStrView() {
    return {nullptr, 0};
}
#endif
