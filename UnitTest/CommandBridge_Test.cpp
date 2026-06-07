#include "stdafx.h"
#include "CommandBridgeClient.h"
#include <string>

class CMapData;
CMapData* Map = nullptr;  // stub — no real map in unit tests

/* ── Test-only proxy extension ─────────────────────────────────────────── */

// Test functions are mock-only and not in the real CommandBridge.h.
// Declare them here so we can resolve them via GetProcAddress.

extern "C" {
    using TestDispatchFn      = int32_t (*)(const char*, RPCB_CallContext*);
    using TestGetActionCountFn = int32_t (*)();
    using TestGetResponseFn    = int32_t (*)(const char*, int32_t*, char*, size_t);
}

class TestProxy : public CommandBridgeProxy {
public:
    bool Load() {
        if (!CommandBridgeProxy::Load("CommandBridgeMock.dll")) {
            return false;
        }
        pTestDispatch       = reinterpret_cast<TestDispatchFn>(
            GetProcAddress(static_cast<HMODULE>(m_hModule), "RPCB_TestDispatch"));
        pTestGetActionCount = reinterpret_cast<TestGetActionCountFn>(
            GetProcAddress(static_cast<HMODULE>(m_hModule), "RPCB_TestGetActionCount"));
        pTestGetResponse    = reinterpret_cast<TestGetResponseFn>(
            GetProcAddress(static_cast<HMODULE>(m_hModule), "RPCB_TestGetResponse"));
        return pTestDispatch && pTestGetActionCount && pTestGetResponse;
    }

    int32_t TestDispatch(const char* name, RPCB_CallContext* ctx) {
        return pTestDispatch ? pTestDispatch(name, ctx) : RPCB_ERR_NOT_INIT;
    }
    int32_t TestGetActionCount() {
        return pTestGetActionCount ? pTestGetActionCount() : -1;
    }
    int32_t TestGetResponse(const char* uid, int32_t* code, char* body, size_t size) {
        return pTestGetResponse ? pTestGetResponse(uid, code, body, size) : RPCB_ERR_NOT_INIT;
    }

private:
    TestDispatchFn       pTestDispatch       = nullptr;
    TestGetActionCountFn pTestGetActionCount = nullptr;
    TestGetResponseFn    pTestGetResponse    = nullptr;
};

/* ── Test callbacks that use RPCB_SendResponse ─────────────────────────── */

static void EchoCallback(RPCB_CallContext* ctx) {
    auto* proxy = static_cast<TestProxy*>(ctx->userData);
    RPCB_StrViewList list;
    list.items = ctx->params.items;
    list.count = ctx->params.count;
    auto emptyErr = EmptyStrView();
    proxy->SendResponse(ctx->uniqueId, emptyErr, &list);
}

static void NullOutputCallback(RPCB_CallContext* ctx) {
    auto* proxy = static_cast<TestProxy*>(ctx->userData);
    auto emptyErr = EmptyStrView();
    proxy->SendResponse(ctx->uniqueId, emptyErr, nullptr);
}

/* ── Test fixture ──────────────────────────────────────────────────────── */

class CommandBridgeTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_TRUE(m_proxy.Load())
            << "CommandBridgeMock.dll not found";
    }

    void TearDown() override {
        if (m_proxy.IsLoaded()) {
            m_proxy.Shutdown();
        }
    }

    TestProxy m_proxy;
};

/* ── Proxy load ────────────────────────────────────────────────────────── */

TEST_F(CommandBridgeTest, ProxyLoadSuccess) {
    EXPECT_TRUE(m_proxy.IsLoaded());
}

TEST_F(CommandBridgeTest, ProxyLoadFail_MissingDll) {
    CommandBridgeProxy proxy;
    EXPECT_FALSE(proxy.Load("nonexistent.dll"));
    EXPECT_FALSE(proxy.IsLoaded());
    EXPECT_EQ(proxy.Shutdown(), RPCB_ERR_NOT_INIT);
}

/* ── Init registers all actions ────────────────────────────────────────── */

TEST_F(CommandBridgeTest, InitRegistersAllActions) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, &m_proxy},
        {"test_null", NullOutputCallback, &m_proxy},
    };
    CommandBridgeInitArgs args = {};
    args.actions     = actions;
    args.actionCount = 2;

    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);
    EXPECT_EQ(m_proxy.TestGetActionCount(), 2);
}

/* ── Init double-call fails ────────────────────────────────────────────── */

TEST_F(CommandBridgeTest, InitDoubleCallFails) {
    CommandBridgeInitArgs args = {};
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);
    EXPECT_EQ(m_proxy.Init(&args), RPCB_ERR_ALREADY_INIT);
}

/* ── Dispatch to callback; verify response via TestGetResponse ─────────── */

TEST_F(CommandBridgeTest, DispatchToCallback) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, &m_proxy},
    };
    CommandBridgeInitArgs args = {};
    args.actions     = actions;
    args.actionCount = 1;
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);

    RPCB_StrView paramItems[2] = {
        {"key", 3}, {"value", 5}
    };
    RPCB_CallContext ctx = {};
    ctx.uniqueId  = {"70000001", 8};
    ctx.paramType = RPCB_PARAM_STRING_LIST;
    ctx.params    = {paramItems, 2};

    ASSERT_EQ(m_proxy.TestDispatch("test_echo", &ctx), RPCB_SUCCESS);

    int32_t statusCode = 0;
    char body[256] = {};
    ASSERT_EQ(m_proxy.TestGetResponse("70000001", &statusCode, body, sizeof(body)), RPCB_SUCCESS);
    EXPECT_EQ(statusCode, 200);
    EXPECT_STRNE(body, "");
}

/* ── Unknown action returns NOT_FOUND ──────────────────────────────────── */

TEST_F(CommandBridgeTest, UnknownActionReturnsNotFound) {
    CommandBridgeInitArgs args = {};
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);

    RPCB_CallContext ctx = {};
    ctx.uniqueId  = {"x", 1};
    ctx.paramType = RPCB_PARAM_STRING_LIST;

    EXPECT_EQ(m_proxy.TestDispatch("nonexistent", &ctx), RPCB_ERR_NOT_FOUND);
}

/* ── Shutdown clears actions ───────────────────────────────────────────── */

TEST_F(CommandBridgeTest, ShutdownClearsActions) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, &m_proxy},
    };
    CommandBridgeInitArgs args = {};
    args.actions     = actions;
    args.actionCount = 1;
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);
    EXPECT_EQ(m_proxy.TestGetActionCount(), 1);

    ASSERT_EQ(m_proxy.Shutdown(), RPCB_SUCCESS);
    EXPECT_EQ(m_proxy.IsRunning(nullptr), 0);
}

/* ── IsRunning reports port ────────────────────────────────────────────── */

TEST_F(CommandBridgeTest, IsRunningReportsPort) {
    CommandBridgeInitArgs args = {};
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);

    int32_t port = 0;
    EXPECT_EQ(m_proxy.IsRunning(&port), 1);
    EXPECT_EQ(port, 4333);
}
