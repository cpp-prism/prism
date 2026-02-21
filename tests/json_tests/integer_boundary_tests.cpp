#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <cstdint>

TEST_CASE("prismJson - integer boundary values", "[json][integer][boundary]")
{
    SECTION("INT_MAX round trip")
    {
        int val = INT_MAX;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == INT_MAX);
    }

    SECTION("INT_MIN round trip")
    {
        int val = INT_MIN;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == INT_MIN);
    }

    SECTION("LLONG_MAX round trip")
    {
        int64_t val = LLONG_MAX;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int64_t>(json);
        REQUIRE(*result == LLONG_MAX);
    }

    SECTION("LLONG_MIN round trip")
    {
        int64_t val = LLONG_MIN;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int64_t>(json);
        REQUIRE(*result == LLONG_MIN);
    }

    SECTION("zero value round trip")
    {
        int val = 0;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 0);
    }

    SECTION("negative one round trip")
    {
        int val = -1;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == -1);
    }

    SECTION("int64 boundary in struct field")
    {
        tst_sub_struct obj;
        obj.my_longlong = LLONG_MAX;
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_longlong == LLONG_MAX);
    }
}
