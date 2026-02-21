#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

TEST_CASE("prismYaml - error input handling", "[yaml][exception]")
{
    SECTION("unterminated quoted string is parsed partially")
    {
        // YAML 解析器不抛出，但 value 会被截断；key 条目仍然存在
        std::string yaml = "key: \"value";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::string>>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->count("key") == 1);
    }

    SECTION("inconsistent indentation: unmatched keys produce default struct")
    {
        // "key" 不是 tst_sub_struct 的字段，整个条目被忽略，struct 保持默认值
        // 注：若 key 匹配字段名，后续错误缩进的行会被拼接到字段值中
        std::string yaml = "key: value\n  invalid indent";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_string.empty());
    }

    SECTION("missing colon: unrecognized line is ignored, struct has default values")
    {
        // 无冒号的行无法解析为 key-value，整行跳过
        std::string yaml = "key value\nanother: test";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_string.empty());
    }

    SECTION("nested keys not matching struct fields produce default values")
    {
        // "key" 不是 tst_sub_struct 的字段，嵌套内容一并忽略
        std::string yaml = "key:\n  nested: value";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_string.empty());
    }

    SECTION("empty input returns default value")
    {
        // 空字符串不抛出，int 返回零值
        std::string yaml = "";
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(*result == 0);
    }

    SECTION("partially invalid flow sequence is parsed")
    {
        // {key: value, invalid} 中 key=value 被成功解析，格式不完整的 "invalid" 条目被忽略
        std::string yaml = R"({key: value, invalid})";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::string>>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE((*result)["key"] == "value");
    }
}
