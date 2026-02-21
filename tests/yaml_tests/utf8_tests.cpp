#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - UTF-8 encoding", "[yaml][utf8]")
{
    SECTION("serialize Chinese characters")
    {
        std::string chinese = "测试中文";
        std::string yaml = prism::yaml::toYamlString(chinese);
        REQUIRE(yaml.find("测试中文") != std::string::npos);
    }

    SECTION("deserialize Chinese characters")
    {
        std::string yaml = "测试中文";
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == "测试中文");
    }

    SECTION("serialize Japanese characters")
    {
        std::string japanese = "テスト日本語";
        std::string yaml = prism::yaml::toYamlString(japanese);
        REQUIRE(yaml.find("テスト日本語") != std::string::npos);
    }

    SECTION("serialize Emoji")
    {
        std::string emoji = "Hello 😀 World";
        std::string yaml = prism::yaml::toYamlString(emoji);
        REQUIRE(yaml.find("Hello") != std::string::npos);
        REQUIRE(yaml.find("World") != std::string::npos);
    }

    SECTION("round trip UTF-8")
    {
        std::string original = "混合测试 Mixed Test αβγ";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }
}
