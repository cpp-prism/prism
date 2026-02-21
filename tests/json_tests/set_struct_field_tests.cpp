#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - set<string> struct field round trip", "[json][set][struct]")
{
    SECTION("set<string> with values round trip")
    {
        tst_struct original;
        original.my_set_str = {"apple", "banana", "cherry"};

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_set_str\"") != std::string::npos);
        REQUIRE(json.find("apple") != std::string::npos);
        REQUIRE(json.find("banana") != std::string::npos);
        REQUIRE(json.find("cherry") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("banana") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
    }

    SECTION("empty set<string> round trip")
    {
        tst_struct original;
        original.my_set_str = {};

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_set_str\":[]") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_set_str.empty());
    }

    SECTION("standalone set<string> round trip")
    {
        std::set<std::string> original = {"x", "y", "z"};
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::set<std::string>>(json);
        REQUIRE(result->size() == 3);
        REQUIRE(result->count("x") == 1);
        REQUIRE(result->count("y") == 1);
        REQUIRE(result->count("z") == 1);
    }
}
