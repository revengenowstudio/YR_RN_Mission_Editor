#include "stdafx.h"
#include "structs.h"
#include <optional>
#include <vector>

TEST(XCStringTest, ConstructorAndConversion) {
    // 1. 纯英文测试（不受编码影响）
    XCString s1("TestNode");
    EXPECT_EQ(s1.len, 8);
    EXPECT_STREQ(s1.cString, "TestNode");
    EXPECT_EQ(wcscmp(s1.wString, L"TestNode"), 0);

    // 2. 中文测试：手动指定 GBK 字节序列，避免编译器干扰
    // "中文测试" 的 GBK 编码: D6 D0 (中), CE C4 (文), B2 E2 (测), CA D4 (试)
    const char gbkData[] = {
        (char)0xD6, (char)0xD0,
        (char)0xCE, (char)0xC4,
        (char)0xB2, (char)0xE2,
        (char)0xCA, (char)0xD4,
        0
    };

    XCString s2(gbkData);

    // 只有在 ACP 为 936 (简体中文) 的环境下，转换结果才是预期的宽字符
    if (GetACP() == 936) {
        EXPECT_EQ(s2.len, 4);
        EXPECT_EQ(wcscmp(s2.wString, L"中文测试"), 0);
    }
    else {
        // 在非中文环境（如 CI）下，它会按单字节转换，虽不是中文但逻辑应自洽
        // 验证它至少成功分配了内存且长度等于字节数
        EXPECT_EQ(s2.len, strlen(gbkData));
        EXPECT_NE(s2.wString, nullptr);
    }
}

TEST(XCStringTest, MoveConstructor) {
    XCString original("MoveMe");
    WCHAR* originalPtr = original.wString;
    size_t originalLen = original.len;

    XCString target(std::move(original));

    EXPECT_EQ(target.wString, originalPtr);
    EXPECT_EQ(target.len, originalLen);
    EXPECT_STREQ(target.cString, "MoveMe");

    EXPECT_EQ(original.wString, nullptr);
    EXPECT_EQ(original.len, 0);
    // CString does not support real right value move
    // So original string still carry the same data
    EXPECT_TRUE(!original.cString.IsEmpty());
}

TEST(XCStringTest, MoveAssignment) {
    XCString s1("OldString");
    XCString s2("NewString");

    WCHAR* s2Ptr = s2.wString;

    s1 = std::move(s2);

    EXPECT_EQ(s1.wString, s2Ptr);
    EXPECT_STREQ(s1.cString, "NewString");
    EXPECT_EQ(s2.wString, nullptr);
}

TEST(XCStringTest, OptionalReturnSimulation) {
    auto Func = []() -> std::optional<XCString> {
        XCString inner("OptionalData");
        return { std::move(inner) };
        };

    auto res = Func();
    ASSERT_TRUE(res.has_value());
    EXPECT_STREQ(res->cString, "OptionalData");
    EXPECT_NE(res->wString, nullptr);
}

TEST(XCStringTest, VectorReallocation) {
    std::vector<XCString> vec;
    vec.reserve(1);
    vec.emplace_back("First");

    WCHAR* firstPtr = vec[0].wString;

    vec.emplace_back("Second");

    bool found = false;
    for (const auto& item : vec) {
        if (item.wString == firstPtr) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST(XCStringTest, SetStringReusability) {
    XCString s("Initial");
    void* firstPtr = s.wString;

    s.SetString("SecondLongerString");
    void* secondPtr = s.wString;

    EXPECT_NE(firstPtr, secondPtr);
    EXPECT_STREQ(s.cString, "SecondLongerString");
    EXPECT_EQ(wcscmp(s.wString, L"SecondLongerString"), 0);
}

TEST(XCStringTest, EmptyAndNullHandling) {
    XCString s1("");
    EXPECT_EQ(s1.len, 0);

    XCString s2(nullptr);
    EXPECT_EQ(s2.len, 0);
    EXPECT_EQ(s2.wString, nullptr);
}

TEST(XCStringTest, SelfMoveAssignment) {
    XCString s("SelfMove");
    WCHAR* ptr = s.wString;

    s = std::move(s);

    EXPECT_EQ(s.wString, ptr);
    EXPECT_STREQ(s.cString, "SelfMove");
}

TEST(XCStringTest, WideCharConstructorAccuracy) {
    // 使用十六进制明确指定：C(43), N(4E), 中(4E2D), 文(6587), 测(6D4B), 试(8BD5)
    const wchar_t wStr[] = { 0x0043, 0x004E, 0x4E2D, 0x6587, 0x6D4B, 0x8BD5, 0x0000 };

    size_t inputLen = wcslen(wStr);
    EXPECT_EQ(inputLen, 6); // `\0` does not count here, so the result is 6

    XCString s(wStr, inputLen);

    EXPECT_EQ(s.len, 6);
    EXPECT_EQ(s.wString[2], 0x4E2D); // 中
    EXPECT_EQ(s.wString[5], 0x8BD5); // 试
    EXPECT_EQ(s.wString[6], 0x0000); // 结尾必须是 \0
}

// 模拟 makeDisplayString 的路径
TEST(XCStringTest, ConcatedStringHandling) {
    // 强制使用宽字符拼装测试，避开 ANSI 编码不一致问题
    CStringW wID = L"Unit01";
    CStringW wName = L"坦克";
    CStringW wFull = wID + L" " + wName;

    XCString s(wFull.GetString(), wFull.GetLength());

    EXPECT_EQ(s.len, 9); // "Unit01 坦克" -> 6 + 1 + 2 = 9
    EXPECT_EQ(s.wString[7], L'坦');
}

TEST(XCStringTest, MultiByteConversionConsistency) {
    // 使用 u8 并在测试中显式处理，或者直接验证转换后的内存不为 NULL
    // 这里的重点是验证 SetString 不会因为多字节输入而崩溃
    const char* utf8Str = reinterpret_cast<const char*>(u8"UTF8字符串");

    XCString s(utf8Str);

    EXPECT_GT(s.len, 0);
    EXPECT_NE(s.wString, nullptr);
    EXPECT_STREQ(s.cString, utf8Str);
}
