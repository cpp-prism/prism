#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

TEST_CASE("prismYaml - negative integers", "[yaml][negative]")
{
    SECTION("serialize negative int")
    {
        REQUIRE(prism::yaml::toYamlString(-42) == "-42");
        REQUIRE(prism::yaml::toYamlString(-123456789) == "-123456789");
        REQUIRE(prism::yaml::toYamlString(0) == "0");
    }

    SECTION("deserialize negative int from yaml")
    {
        auto result1 = prism::yaml::fromYamlString<int>("-42");
        REQUIRE(*result1 == -42);

        auto result2 = prism::yaml::fromYamlString<int>("-123456789");
        REQUIRE(*result2 == -123456789);
    }

    SECTION("round trip negative int")
    {
        int original = -999;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismYaml - negative floats", "[yaml][negative][float]")
{
    SECTION("serialize negative float")
    {
        // YAML 使用 setprecision(17)，只验证结果含负号且数值正确（round-trip）
        double val = -3.14159;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("-") != std::string::npos);
        auto result = prism::yaml::fromYamlString<double>(yaml);
        REQUIRE(std::abs(*result - val) < 0.000001);
    }

    SECTION("deserialize negative float from yaml")
    {
        auto result = prism::yaml::fromYamlString<double>("-2.71828");
        REQUIRE(std::abs(*result - (-2.71828)) < 0.00001);
    }

    SECTION("serialize negative double")
    {
        double val = -1.7976931348623157e+308;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("-") != std::string::npos);
    }
}
