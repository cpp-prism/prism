#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - tst_sub_struct my_longlong and my_opt_int combinations round trip", "[json][sub_struct][longlong][opt_int]")
{
    SECTION("tst_sub_struct max longlong and positive opt_int round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_longlong = LLONG_MAX;
        obj.my_opt_int = INT_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MAX);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MAX);
    }

    SECTION("tst_sub_struct min longlong and negative opt_int round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_longlong = LLONG_MIN;
        obj.my_opt_int = INT_MIN;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MIN);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MIN);
    }

    SECTION("tst_sub_struct zero longlong and nullopt round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_longlong = 0LL;
        obj.my_opt_int = std::nullopt;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == 0LL);
        REQUIRE(!result->my_opt_int.has_value());
    }
}
