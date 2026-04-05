#include "stdafx.h"
#include "structs.h"
#include <optional>
#include <vector>

TEST(XCStringTest, ConstructorAndConversion) {
    XCString s1("TestNode");
    EXPECT_EQ(s1.len, 8);
    EXPECT_STREQ(s1.cString, "TestNode");
    EXPECT_EQ(wcscmp(s1.wString, L"TestNode"), 0);

    XCString s2("中文测试");
    EXPECT_EQ(wcscmp(s2.wString, L"中文测试"), 0);
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

TEST(XCStringTest, ChineseCharacterLengthAccuracy) {
    // 在 GBK/ANSI 编码下：
    // "CN中文" 的字节数 (strlen) 是 2 + 2*2 = 6
    // "CN中文" 的字符数 (WCHAR count) 是 2 + 2 = 4
    const char* mixedStr = "CN中文";

    XCString s(mixedStr);

    // 验证 len 必须是字符数 4，而不是 strlen 的 6
    EXPECT_EQ(s.len, 4);

    // 验证内存分配是否紧凑 (应分配 5 个 WCHAR)
    // 验证转换后的内容
    EXPECT_EQ(s.wString[0], L'C');
    EXPECT_EQ(s.wString[1], L'N');
    EXPECT_EQ(s.wString[2], L'中');
    EXPECT_EQ(s.wString[3], L'文');
    EXPECT_EQ(s.wString[4], L'\0');
}

TEST(XCStringTest, MultiByteConversionRobustness) {
    // 测试更复杂的混合场景
    const char* complexStr = "A加B等于C";
    // strlen: 1 + 2 + 1 + 2 + 2 + 1 = 9 字节
    // WCHAR count: 6 字符

    XCString s(complexStr);

    EXPECT_EQ(s.len, 6);
    EXPECT_STREQ(s.cString, complexStr);
}

TEST(XCStringTest, BufferSafetyWithLongChinese) {
    // 确保长中文字符串不会导致溢出或截断
    std::string longChinese(100, 'a'); // 100个英文字符
    longChinese += "测试内容"; // 加上 4 个中文字符 (8字节)

    XCString s(longChinese.c_str());

    // 100个 'a' + 4个汉字 = 104 字符
    EXPECT_EQ(s.len, 104);
    EXPECT_EQ(s.wString[100], L'测');
}
