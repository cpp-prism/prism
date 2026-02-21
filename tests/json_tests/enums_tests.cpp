#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismJson - enum serialization", "[json][enums]")
{
    SECTION("serialize enum to json")
    {
        // 测试 language 枚举序列化
        std::string json = prism::json::toJsonString(language::english);
        REQUIRE(json == "\"英语\"");
    }

    SECTION("deserialize enum from json")
    {
        // 测试从 JSON 反序列化枚举
        std::string json = "\"简体中文\"";
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == language::SimplifiedChinese);
    }

    SECTION("deserialize unknow enum from json")
    {
        // 测试反序列化未知枚举值
        std::string json = "\"未知\"";
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == language::unknow);
    }
}

TEST_CASE("prismJson - enum round trip", "[json][enums]")
{
    SECTION("round trip enum english")
    {
        language original = language::english;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum SimplifiedChinese")
    {
        language original = language::SimplifiedChinese;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum TraditionalChinese")
    {
        language original = language::TraditionalChinese;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum unknow")
    {
        language original = language::unknow;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<language>(json);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismJson - enum in struct", "[json][enums]")
{
    SECTION("serialize struct with enum")
    {
        // 使用 tst_struct 中的 lang 字段测试
        tst_struct obj;
        obj.lang = language::english;

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"lang\":\"英语\"") != std::string::npos);
    }

    SECTION("deserialize struct with enum")
    {
        std::string json = R"({"lang":"简体中文"})";
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->lang == language::SimplifiedChinese);
    }

    SECTION("round trip struct with enum")
    {
        tst_struct original;
        original.lang = language::TraditionalChinese;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->lang == original.lang);
    }
}
