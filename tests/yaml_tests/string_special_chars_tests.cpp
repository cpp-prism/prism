#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - string with YAML special characters", "[yaml][string][special]")
{
    SECTION("string with colon round trip")
    {
        // 包含冒号的字符串需要引号
        std::string original = "key: value";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("string with hash round trip")
    {
        // 包含井号的字符串（注释符号）
        std::string original = "color: #FF0000";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("string with double quote round trip")
    {
        std::string original = R"(say "hello world")";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("string with backslash round trip")
    {
        std::string original = "path\\to\\file";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("string in struct with colon character")
    {
        tst_sub_struct obj;
        obj.my_string = "server: localhost:8080";
        std::string yaml = prism::yaml::toYamlString(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_string == obj.my_string);
    }

    SECTION("string starting with dash round trip")
    {
        // 以 '-' 开头的字符串在 YAML 中需要引号（否则被当作列表项）
        std::string original = "- list item lookalike";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }
}
