#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

TEST_CASE("prismJson - error input handling", "[json][exception]")
{
    SECTION("unquoted key: key-value pair is silently rejected")
    {
        // 解析器要求 key 必须带引号，遇到裸 key 时静默跳过，返回空 map
        std::string json = "{key: \"value\"}";
        auto result = prism::json::fromJsonString<std::map<std::string, std::string>>(json);
        REQUIRE(result != nullptr);
        REQUIRE(result->empty());
    }

    SECTION("unterminated string throws")
    {
        // 解析器抛出 const char* 类型错误
        std::string json = R"({"key": "value)";
        REQUIRE_THROWS(prism::json::fromJsonString<std::map<std::string, std::string>>(json));
    }

    SECTION("unterminated object throws")
    {
        // 缺少闭合 '}'，解析器抛出 const char* 类型错误
        std::string json = R"({"key": "value")";
        REQUIRE_THROWS(prism::json::fromJsonString<std::map<std::string, std::string>>(json));
    }

    SECTION("trailing garbage after number is silently ignored")
    {
        // from_chars 读取 123 后在 'a' 处停止；解析器不报错，取得有效数值
        std::string json = R"({"key": 123a})";
        auto result = prism::json::fromJsonString<std::map<std::string, int>>(json);
        REQUIRE(result != nullptr);
        REQUIRE((*result)["key"] == 123);
    }

    SECTION("missing colon: key-value pair is silently skipped")
    {
        // 解析器找不到 ':' 时跳过整个 key-value，返回空 map
        std::string json = R"({"key" "value"})";
        auto result = prism::json::fromJsonString<std::map<std::string, std::string>>(json);
        REQUIRE(result != nullptr);
        REQUIRE(result->empty());
    }

    SECTION("trailing comma is accepted")
    {
        // 解析器对尾随逗号宽松处理，正常解析已有的 key-value
        std::string json = R"({"key": "value",})";
        auto result = prism::json::fromJsonString<std::map<std::string, std::string>>(json);
        REQUIRE(result != nullptr);
        REQUIRE((*result)["key"] == "value");
    }

    SECTION("empty input returns default value")
    {
        // 空字符串不抛出，int 返回零值
        std::string json = "";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(result != nullptr);
        REQUIRE(*result == 0);
    }

    SECTION("null token returns default value for int")
    {
        // "null" 不抛出，int 返回零值
        std::string json = "null";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(result != nullptr);
        REQUIRE(*result == 0);
    }
}
