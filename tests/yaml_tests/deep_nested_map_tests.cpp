#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

TEST_CASE("prismYaml - deeply nested map containers", "[yaml][container][nested]")
{
    SECTION("map<string, map<string, int>> serialization")
    {
        std::map<std::string, std::map<std::string, int>> val = {
            {"outer1", {{"inner1", 1}, {"inner2", 2}}},
            {"outer2", {{"inner3", 3}}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("outer1") != std::string::npos);
        REQUIRE(yaml.find("inner1") != std::string::npos);
        REQUIRE(yaml.find("inner3") != std::string::npos);
    }

    SECTION("map<string, map<string, int>> round trip")
    {
        std::map<std::string, std::map<std::string, int>> original = {
            {"outer1", {{"inner1", 10}, {"inner2", 20}}},
            {"outer2", {{"inner3", 30}}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::map<std::string, int>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["outer1"].size() == 2);
        REQUIRE((*result)["outer1"]["inner1"] == 10);
        REQUIRE((*result)["outer1"]["inner2"] == 20);
        REQUIRE((*result)["outer2"]["inner3"] == 30);
    }

    SECTION("map<string, map<string, string>> round trip")
    {
        std::map<std::string, std::map<std::string, std::string>> original = {
            {"group1", {{"key_a", "value_a"}, {"key_b", "value_b"}}},
            {"group2", {{"key_c", "value_c"}}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::map<std::string, std::string>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["group1"]["key_a"] == "value_a");
        REQUIRE((*result)["group1"]["key_b"] == "value_b");
        REQUIRE((*result)["group2"]["key_c"] == "value_c");
    }

    SECTION("vector<map<string, map<string, int>>> complex nesting")
    {
        std::vector<std::map<std::string, std::map<std::string, int>>> original = {
            {{"outer", {{"inner", 42}}}},
            {{"another", {{"deep", 99}}}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<std::map<std::string, std::map<std::string, int>>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0]["outer"]["inner"] == 42);
        REQUIRE((*result)[1]["another"]["deep"] == 99);
    }
}
