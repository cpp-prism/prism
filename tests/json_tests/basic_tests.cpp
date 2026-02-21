#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismJson - basic types", "[json]")
{
    SECTION("serialize int")
    {
        int val = 42;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "42");
    }

    SECTION("deserialize int from json")
    {
        std::string json = "123";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 123);
    }

    SECTION("serialize double")
    {
        double val = 3.14;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("3.14") != std::string::npos);
    }

    SECTION("serialize bool")
    {
        bool val = true;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "true");
    }

    SECTION("deserialize bool from json")
    {
        std::string json = "false";
        auto result = prism::json::fromJsonString<bool>(json);
        REQUIRE(*result == false);
    }
}

TEST_CASE("prismJson - round trip", "[json]")
{
    SECTION("round trip int")
    {
        int original = 999;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == original);
    }

    SECTION("round trip string")
    {
        std::string original = "test round trip";
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("round trip bool")
    {
        bool original = true;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<bool>(json);
        REQUIRE(*result == original);
    }
}
