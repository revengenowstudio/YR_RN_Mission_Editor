#include "stdafx.h"
#include "CommandBridgeClient.h"
#include "CommandBridgeClient/Handler.h"
#include <string>

class CMapData;
CMapData* Map = nullptr;  // stub — no real map in unit tests

// Stub callbacks — test doesn't exercise real trigger/tag logic
#define STUB_CALLBACK(name) void name(RPCB_CallContext*) {}
STUB_CALLBACK(OnTriggerList)
STUB_CALLBACK(OnTriggerGetBasic)
STUB_CALLBACK(OnTriggerEventCount)
STUB_CALLBACK(OnTriggerEventGet)
STUB_CALLBACK(OnTriggerActionCount)
STUB_CALLBACK(OnTriggerActionGet)
STUB_CALLBACK(OnTriggerCreate)
STUB_CALLBACK(OnTriggerUpdate)
STUB_CALLBACK(OnTriggerDelete)
STUB_CALLBACK(OnTagList)
STUB_CALLBACK(OnTagGet)
STUB_CALLBACK(OnTagCreate)
STUB_CALLBACK(OnTagUpdate)
STUB_CALLBACK(OnTagDelete)
STUB_CALLBACK(OnUpdateDialogs)
#undef STUB_CALLBACK

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
    RPCB_Response resp = {RPCB_PARAM_STRING_LIST, EmptyStrView(), {ctx->params.items, ctx->params.count}};
    proxy->SendResponse(ctx->uniqueId, &resp);
}

static void NullOutputCallback(RPCB_CallContext* ctx) {
    auto* proxy = static_cast<TestProxy*>(ctx->userData);
    RPCB_Response resp = {RPCB_PARAM_STRING_LIST, EmptyStrView(), {}};
    proxy->SendResponse(ctx->uniqueId, &resp);
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

/* ── ParamExtractor tests ──────────────────────────────────────────────── */

TEST(ParamExtractorTest, EmptyParams) {
    ParamExtractor ex;
    int called = 0;
    ex.Add("id", [&](std::string_view) { called++; });
    RPCB_StrViewList params = {nullptr, 0};
    ex.Extract(params);
    EXPECT_EQ(called, 0);
}

TEST(ParamExtractorTest, SingleFieldMatch) {
    ParamExtractor ex;
    std::string result;
    ex.Add("id", [&](std::string_view v) { result = std::string(v); });

    const char* idVal = "70000001";
    RPCB_StrView items[2] = {
        ToStrView("id"), {idVal, 8}
    };
    RPCB_StrViewList params = {items, 2};
    ex.Extract(params);
    EXPECT_EQ(result, "70000001");
}

TEST(ParamExtractorTest, MultipleFieldsSorted) {
    ParamExtractor ex;
    std::string a, b, c;
    ex.Add("house", [&](std::string_view v) { a = std::string(v); });
    ex.Add("id",    [&](std::string_view v) { b = std::string(v); });
    ex.Add("name",  [&](std::string_view v) { c = std::string(v); });

    const char* houseVal = "British";
    const char* idVal    = "70000001";
    const char* nameVal  = "Player Arrives";
    RPCB_StrView items[6] = {
        {"house", 5}, {houseVal, 7},
        {"id",    2}, {idVal,    8},
        {"name",  4}, {nameVal, 14},
    };
    RPCB_StrViewList params = {items, 6};
    ex.Extract(params);
    EXPECT_EQ(a, "British");
    EXPECT_EQ(b, "70000001");
    EXPECT_EQ(c, "Player Arrives");
}

TEST(ParamExtractorTest, FieldsAddedOutOfOrder) {
    // Add in non-alphabetical order; extraction should still work
    ParamExtractor ex;
    std::string first, second, third;
    ex.Add("name",  [&](std::string_view v) { first  = std::string(v); });
    ex.Add("id",    [&](std::string_view v) { second = std::string(v); });
    ex.Add("house", [&](std::string_view v) { third  = std::string(v); });

    const char* houseVal = "British";
    const char* idVal    = "70000001";
    const char* nameVal  = "Player Arrives";
    RPCB_StrView items[6] = {
        {"house", 5}, {houseVal, 7},
        {"id",    2}, {idVal,    8},
        {"name",  4}, {nameVal, 14},
    };
    RPCB_StrViewList params = {items, 6};
    ex.Extract(params);
    EXPECT_EQ(first,  "Player Arrives");
    EXPECT_EQ(second, "70000001");
    EXPECT_EQ(third,  "British");
}

TEST(ParamExtractorTest, ExtraUnknownParamsSkipped) {
    ParamExtractor ex;
    std::string result;
    ex.Add("name", [&](std::string_view v) { result = std::string(v); });

    const char* nameVal = "Test";
    RPCB_StrView items[6] = {
        {"extra1", 6}, {"val1",   4},
        {"extra2", 6}, {"val2",   4},
        {"name",   4}, {nameVal, 4},
    };
    RPCB_StrViewList params = {items, 6};
    ex.Extract(params);
    EXPECT_EQ(result, "Test");
}

TEST(ParamExtractorTest, MissingFieldsNotCalled) {
    ParamExtractor ex;
    int calledA = 0, calledB = 0;
    ex.Add("present", [&](std::string_view) { calledA++; });
    ex.Add("absent",  [&](std::string_view) { calledB++; });

    const char* val = "x";
    RPCB_StrView items[2] = {{"present", 7}, {val, 1}};
    RPCB_StrViewList params = {items, 2};
    ex.Extract(params);
    EXPECT_EQ(calledA, 1);
    EXPECT_EQ(calledB, 0);
}
