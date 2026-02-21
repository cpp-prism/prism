#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - set<string> struct field round trip", "[yaml][set][struct]")
{
    SECTION("tst_struct my_set_str with values block round trip")
    {
        tst_struct original;
        original.my_set_str = {"apple", "banana", "cherry"};

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("banana") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
    }

    SECTION("tst_struct my_set_str empty block round trip")
    {
        tst_struct original;
        // my_set_str is empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.empty());
    }

    SECTION("standalone set<string> block round trip")
    {
        std::set<std::string> original = {"x", "y", "z"};

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<std::set<std::string>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE(result->count("x") == 1);
        REQUIRE(result->count("y") == 1);
        REQUIRE(result->count("z") == 1);
    }

    SECTION("standalone set<string> flow round trip")
    {
        std::set<std::string> original = {"foo", "bar"};

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::set<std::string>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE(result->count("foo") == 1);
        REQUIRE(result->count("bar") == 1);
    }
}
