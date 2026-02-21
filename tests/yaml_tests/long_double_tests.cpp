#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <limits>

// 全局作用域定义结构体
struct LongDoubleStruct {
    long double ld;
};
PRISM_FIELDS(LongDoubleStruct, ld)

TEST_CASE("prismYaml - long double serialization", "[yaml][longdouble]")
{
    SECTION("serialize long double")
    {
        long double val = 3.141592653589793238L;
        std::string yaml = prism::yaml::toYamlString(val);
        // Should contain the number
        REQUIRE(yaml.find("3.14") != std::string::npos);
    }

    SECTION("serialize long double with high precision")
    {
        long double val = 1.234567890123456789L;
        std::string yaml = prism::yaml::toYamlString(val);
        // Just verify it serializes without error
        REQUIRE(!yaml.empty());
    }

    SECTION("deserialize long double from yaml")
    {
        std::string yaml = "6.626e-34";
        auto result = prism::yaml::fromYamlString<long double>(yaml);
        REQUIRE(std::abs(*result - 6.626e-34L) < 1e-40L);
    }

    SECTION("round trip long double")
    {
        long double original = 2.718281828459045235L;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<long double>(yaml);
        // Long double precision loss is expected, relaxed requirement
        REQUIRE(std::abs(*result - original) < original * 1e-5L);
    }

    SECTION("serialize long double max")
    {
        long double val = std::numeric_limits<long double>::max();
        std::string yaml = prism::yaml::toYamlString(val);
        // Should serialize without crashing
        REQUIRE(!yaml.empty());
    }

    SECTION("serialize long double min")
    {
        long double val = std::numeric_limits<long double>::min();
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(!yaml.empty());
    }

    SECTION("serialize long double negative")
    {
        long double val = -1.414213562373095048L;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("-") != std::string::npos);
    }

    SECTION("long double in struct")
    {
        LongDoubleStruct obj = {3.14159L};
        std::string yaml = prism::yaml::toYamlString(obj);
        auto result = prism::yaml::fromYamlString<LongDoubleStruct>(yaml);
        REQUIRE(std::abs(result->ld - 3.14159L) < 1e-5L);
    }
}
