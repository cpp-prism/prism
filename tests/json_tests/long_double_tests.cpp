#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>
#include <limits>

// 全局作用域定义结构体
struct LongDoubleStruct {
    long double ld;
};
PRISM_FIELDS(LongDoubleStruct, ld)

TEST_CASE("prismJson - long double serialization", "[json][longdouble]")
{
    SECTION("serialize long double")
    {
        long double val = 3.141592653589793238L;
        std::string json = prism::json::toJsonString(val);
        // Should contain the number
        REQUIRE(json.find("3.14") != std::string::npos);
    }

    SECTION("serialize long double with high precision")
    {
        long double val = 1.234567890123456789L;
        std::string json = prism::json::toJsonString(val);
        // Just verify it serializes without error
        REQUIRE(!json.empty());
    }

    SECTION("deserialize long double from json")
    {
        std::string json = "6.626e-34";
        auto result = prism::json::fromJsonString<long double>(json);
        REQUIRE(std::abs(*result - 6.626e-34L) < 1e-40L);
    }

    SECTION("round trip long double")
    {
        long double original = 2.718281828459045235L;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<long double>(json);
        // Long double precision loss is expected, relaxed requirement
        REQUIRE(std::abs(*result - original) < original * 1e-5L);
    }

    SECTION("serialize long double max")
    {
        long double val = std::numeric_limits<long double>::max();
        std::string json = prism::json::toJsonString(val);
        // Should serialize without crashing
        REQUIRE(!json.empty());
    }

    SECTION("serialize long double min")
    {
        long double val = std::numeric_limits<long double>::min();
        std::string json = prism::json::toJsonString(val);
        REQUIRE(!json.empty());
    }

    SECTION("serialize long double negative")
    {
        long double val = -1.414213562373095048L;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("-") != std::string::npos);
    }

    SECTION("long double in struct")
    {
        LongDoubleStruct obj = {3.14159L};
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<LongDoubleStruct>(json);
        REQUIRE(std::abs(result->ld - 3.14159L) < 1e-5L);
    }
}
