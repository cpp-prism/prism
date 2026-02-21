#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismYaml - enum serialization", "[yaml][enums]")
{
    SECTION("serialize enum to yaml")
    {
        // 测试 language 枚举序列化
        // YAML 序列化枚举时不加引号
        std::string yaml = prism::yaml::toYamlString(language::english);
        REQUIRE(yaml == "英语");
    }

    SECTION("deserialize enum from yaml (quoted)")
    {
        // 测试从 YAML 反序列化枚举（带引号）
        // 注意：YAML 流格式解析对中文字符串支持有限，
        // 建议使用带引号的格式
        std::string yaml = "\"简体中文\"";
        auto result = prism::yaml::fromYamlString<language>(yaml);
        REQUIRE(*result == language::SimplifiedChinese);
    }

    SECTION("deserialize unknow enum from yaml")
    {
        // 测试反序列化未知枚举值
        std::string yaml = "\"未知\"";
        auto result = prism::yaml::fromYamlString<language>(yaml);
        REQUIRE(*result == language::unknow);
    }
}

TEST_CASE("prismYaml - enum round trip", "[yaml][enums]")
{
    SECTION("round trip enum english")
    {
        language original = language::english;
        std::string yaml = prism::yaml::toYamlString(original);
        // 使用带引号的格式进行 round-trip
        std::string yaml_quoted = "\"" + yaml + "\"";
        auto result = prism::yaml::fromYamlString<language>(yaml_quoted);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum SimplifiedChinese")
    {
        language original = language::SimplifiedChinese;
        std::string yaml = prism::yaml::toYamlString(original);
        std::string yaml_quoted = "\"" + yaml + "\"";
        auto result = prism::yaml::fromYamlString<language>(yaml_quoted);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum TraditionalChinese")
    {
        language original = language::TraditionalChinese;
        std::string yaml = prism::yaml::toYamlString(original);
        std::string yaml_quoted = "\"" + yaml + "\"";
        auto result = prism::yaml::fromYamlString<language>(yaml_quoted);
        REQUIRE(*result == original);
    }

    SECTION("round trip enum unknow")
    {
        language original = language::unknow;
        std::string yaml = prism::yaml::toYamlString(original);
        std::string yaml_quoted = "\"" + yaml + "\"";
        auto result = prism::yaml::fromYamlString<language>(yaml_quoted);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismYaml - enum in struct", "[yaml][enums]")
{
    SECTION("serialize struct with enum")
    {
        // 使用 tst_struct 中的 lang 字段测试
        tst_struct obj;
        obj.lang = language::english;

        std::string yaml = prism::yaml::toYamlString(obj);
        REQUIRE(yaml.find("lang: 英语") != std::string::npos);
    }

    SECTION("deserialize struct with enum")
    {
        std::string yaml = "lang: \"简体中文\"";
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->lang == language::SimplifiedChinese);
    }

    SECTION("round trip struct with enum")
    {
        tst_struct original;
        original.lang = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->lang == original.lang);
    }
}
