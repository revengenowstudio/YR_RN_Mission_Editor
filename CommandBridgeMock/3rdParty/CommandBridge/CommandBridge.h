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
    RPCB_StrView* items;
    size_t        count;
} RPCB_StrViewList;

/* --------------------------------------------------------------------------
   Parameter type enum
   -------------------------------------------------------------------------- */

typedef enum RPCB_ParamType {
    RPCB_PARAM_STRING_LIST = 0,
} RPCB_ParamType;

/* --------------------------------------------------------------------------
   Call context & callback signature
   -------------------------------------------------------------------------- */

typedef struct RPCB_CallContext {
    RPCB_StrView    uniqueId;
    RPCB_ParamType  paramType;
    RPCB_StrViewList params;
    RPCB_StrViewList output;
    int32_t         statusCode;
    void*           userData;
} RPCB_CallContext;

typedef void (*RPCB_ActionCallback)(RPCB_CallContext* ctx);

/* --------------------------------------------------------------------------
   Init args
   -------------------------------------------------------------------------- */

typedef struct CommandBridgeActionDef {
    const char*          name;
    RPCB_ActionCallback  callback;
    void*                userData;
} CommandBridgeActionDef;

typedef struct CommandBridgeInitArgs {
    int32_t                       reserved;
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

/* ── Test-only functions (CommandBridgeMock only) ─────────────────────── */

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestDispatch(const char* actionName,
                                                 RPCB_CallContext* ctx);

COMMAND_BRIDGE_EXPORT int32_t RPCB_TestGetActionCount(void);

#ifdef __cplusplus
}
#endif
