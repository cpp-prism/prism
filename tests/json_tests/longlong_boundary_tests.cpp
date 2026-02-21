#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_longlong boundary values round trip", "[json][longlong][boundary]")
{
    SECTION("my_longlong max value round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_longlong = LLONG_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MAX);
    }

    SECTION("my_longlong min value round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_longlong = LLONG_MIN;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MIN);
    }

    SECTION("my_longlong zero and negative values round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_longlong = 0LL;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_longlong == 0LL);

        obj.my_longlong = -1234567890123456789LL;
        json = prism::json::toJsonString(obj);
        result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_longlong == -1234567890123456789LL);
    }
}
