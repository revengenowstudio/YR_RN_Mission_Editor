#include "stdafx.h"
#include "CommandBridgeClient.h"
#include <string>

/* ── Test callback that echoes input back as output ────────────────────── */

static void EchoCallback(RPCB_CallContext* ctx) {
    // Copy input params to output (echo)
    if (ctx->params.count > 0) {
        ctx->output.count = ctx->params.count;
        ctx->output.items = ctx->params.items;
    }
    ctx->statusCode = 200;
}

static void NullOutputCallback(RPCB_CallContext* ctx) {
    ctx->output.count = 0;
    ctx->statusCode = 204;
}

/* ── Test fixture ──────────────────────────────────────────────────────── */

class CommandBridgeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load mock DLL from the same output directory
        ASSERT_TRUE(m_proxy.Load("CommandBridgeMockd.dll"))
            << "CommandBridgeMockd.dll not found";
    }

    void TearDown() override {
        if (m_proxy.IsLoaded()) {
            m_proxy.Shutdown();
        }
    }

    CommandBridgeProxy m_proxy;
};

/* ── Proxy load ────────────────────────────────────────────────────────── */

TEST_F(CommandBridgeTest, ProxyLoadSuccess) {
    EXPECT_TRUE(m_proxy.IsLoaded());
}

TEST_F(CommandBridgeTest, ProxyLoadFail_MissingDll) {
    CommandBridgeProxy proxy;
    EXPECT_FALSE(proxy.Load("nonexistent.dll"));
    EXPECT_FALSE(proxy.IsLoaded());
    // CALL_IMPORT_FUNC should return default value when not loaded
    EXPECT_EQ(proxy.Shutdown(), RPCB_ERR_NOT_INIT);
}

/* ── Init registers all actions ────────────────────────────────────────── */

TEST_F(CommandBridgeTest, InitRegistersAllActions) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, nullptr},
        {"test_null", NullOutputCallback, nullptr},
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

/* ── Dispatch to callback ──────────────────────────────────────────────── */

TEST_F(CommandBridgeTest, DispatchToCallback) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, nullptr},
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
    EXPECT_EQ(ctx.statusCode, 200);
    EXPECT_EQ(ctx.output.count, 2u);
    // Output should echo input
    EXPECT_EQ(std::string(ctx.output.items[0].data, ctx.output.items[0].len), "key");
    EXPECT_EQ(std::string(ctx.output.items[1].data, ctx.output.items[1].len), "value");
}

/* ── Unknown action returns 404 ────────────────────────────────────────── */

TEST_F(CommandBridgeTest, UnknownActionReturns404) {
    CommandBridgeInitArgs args = {};
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);

    RPCB_CallContext ctx = {};
    ctx.uniqueId  = {"x", 1};
    ctx.paramType = RPCB_PARAM_STRING_LIST;

    ASSERT_EQ(m_proxy.TestDispatch("nonexistent", &ctx), RPCB_SUCCESS);
    EXPECT_EQ(ctx.statusCode, 404);
}

/* ── Shutdown clears actions ───────────────────────────────────────────── */

TEST_F(CommandBridgeTest, ShutdownClearsActions) {
    CommandBridgeActionDef actions[] = {
        {"test_echo", EchoCallback, nullptr},
    };
    CommandBridgeInitArgs args = {};
    args.actions     = actions;
    args.actionCount = 1;
    ASSERT_EQ(m_proxy.Init(&args), RPCB_SUCCESS);
    EXPECT_EQ(m_proxy.TestGetActionCount(), 1);

    ASSERT_EQ(m_proxy.Shutdown(), RPCB_SUCCESS);
    // After shutdown, IsRunning returns 0
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
