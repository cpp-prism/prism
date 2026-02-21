#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - UTF-8 encoding", "[json][utf8]")
{
    SECTION("serialize Chinese characters")
    {
        std::string chinese = "测试中文";
        std::string json = prism::json::toJsonString(chinese);
        REQUIRE(json.find("测试中文") != std::string::npos);
    }

    SECTION("deserialize Chinese characters")
    {
        std::string json = "\"测试中文\"";
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == "测试中文");
    }

    SECTION("serialize Japanese characters")
    {
        std::string japanese = "テスト日本語";
        std::string json = prism::json::toJsonString(japanese);
        REQUIRE(json.find("テスト日本語") != std::string::npos);
    }

    SECTION("serialize Emoji")
    {
        std::string emoji = "Hello 😀 World";
        std::string json = prism::json::toJsonString(emoji);
        REQUIRE(json.find("Hello") != std::string::npos);
        REQUIRE(json.find("World") != std::string::npos);
    }

    SECTION("round trip UTF-8")
    {
        std::string original = "混合测试 Mixed Test αβγ";
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }
}
