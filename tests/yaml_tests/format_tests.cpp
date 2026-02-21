#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - format modes", "[yaml][format]")
{
    SECTION("all_block mode for struct")
    {
        tst_sub_struct val;
        val.my_int = 123;
        val.my_bool = true;
        val.my_string = "test";
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        // block 格式不使用 '{...}' 包裹结构体（空数组用 '[]' 是合法的）
        REQUIRE(yaml.find("{") == std::string::npos);
        REQUIRE(yaml.find("my_int:") != std::string::npos);
        REQUIRE(yaml.find("my_bool:") != std::string::npos);
    }

    SECTION("all_flow mode for struct")
    {
        tst_sub_struct val;
        val.my_int = 123;
        val.my_bool = true;
        val.my_string = "test";
        std::string yaml = prism::yaml::toYamlStringFlow(val, 2);
        REQUIRE(yaml.find("{") != std::string::npos);
        REQUIRE(yaml.find("}") != std::string::npos);
    }

    SECTION("all_block mode for vector")
    {
        std::vector<int> val = {1, 2, 3};
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        REQUIRE(yaml.find("- 1") != std::string::npos);
        REQUIRE(yaml.find("[") == std::string::npos);
    }

    SECTION("all_flow mode for vector")
    {
        std::vector<int> val = {1, 2, 3};
        std::string yaml = prism::yaml::toYamlStringFlow(val, 2);
        REQUIRE(yaml.find("[1, 2, 3]") != std::string::npos);
    }

    SECTION("all_block mode for map")
    {
        std::map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        REQUIRE(yaml.find("{") == std::string::npos);
        REQUIRE(yaml.find("a:") != std::string::npos);
    }

    SECTION("all_flow mode for map")
    {
        std::map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string yaml = prism::yaml::toYamlStringFlow(val, 2);
        REQUIRE(yaml.find("{") != std::string::npos);
    }

    SECTION("serialize unordered_map<string, int> block mode")
    {
        std::unordered_map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        REQUIRE(yaml.find("a:") != std::string::npos);
        REQUIRE(yaml.find("b:") != std::string::npos);
    }

    SECTION("serialize unordered_map<string, int> flow mode")
    {
        std::unordered_map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string yaml = prism::yaml::toYamlStringFlow(val, 2);
        REQUIRE(yaml.find("{") != std::string::npos);
    }

    SECTION("deserialize set<int>")
    {
        std::string yaml = "[1, 2, 3]";
        auto result = prism::yaml::fromYamlString<std::set<int>>(yaml);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize set<string>")
    {
        std::string yaml = R"(["a", "b", "c"])";
        auto result = prism::yaml::fromYamlString<std::set<std::string>>(yaml);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize deque<float>")
    {
        std::string yaml = "[1.1, 2.2, 3.3]";
        auto result = prism::yaml::fromYamlString<std::deque<float>>(yaml);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize deque<int>")
    {
        std::string yaml = "[1, 2, 3]";
        auto result = prism::yaml::fromYamlString<std::deque<int>>(yaml);
        REQUIRE(result->size() == 3);
    }

}
