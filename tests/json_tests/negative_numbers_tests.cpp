#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - negative integers", "[json][negative]")
{
    SECTION("serialize negative int")
    {
        REQUIRE(prism::json::toJsonString(-42) == "-42");
        REQUIRE(prism::json::toJsonString(-123456789) == "-123456789");
        REQUIRE(prism::json::toJsonString(0) == "0");
    }

    SECTION("deserialize negative int from json")
    {
        auto result1 = prism::json::fromJsonString<int>("-42");
        REQUIRE(*result1 == -42);

        auto result2 = prism::json::fromJsonString<int>("-123456789");
        REQUIRE(*result2 == -123456789);
    }

    SECTION("round trip negative int")
    {
        int original = -999;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismJson - negative floats", "[json][negative][float]")
{
    SECTION("serialize negative float")
    {
        std::string json = prism::json::toJsonString(-3.14159);
        REQUIRE(json.find("-3.14159") != std::string::npos);
    }

    SECTION("deserialize negative float from json")
    {
        auto result = prism::json::fromJsonString<double>("-2.71828");
        REQUIRE(std::abs(*result - (-2.71828)) < 0.00001);
    }

    SECTION("serialize negative double")
    {
        double val = -1.7976931348623157e+308;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("-") != std::string::npos);
    }
}
