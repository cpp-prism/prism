#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - default-constructed struct round trip keeps defaults", "[json][default][round_trip]")
{
    SECTION("tst_sub_struct default values round trip")
    {
        tst_sub_struct obj;
        // All fields are at default values

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_float == 0.0f);
        REQUIRE(result->my_double == 0.0);
        REQUIRE(result->my_string.empty());
        REQUIRE(result->my_longlong == 0LL);
    }

    SECTION("level3 default values round trip")
    {
        level3 obj;
        // All fields at default

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level3>(json);

        REQUIRE(result->val == 0);
        REQUIRE(result->name.empty());
    }

    SECTION("Derived default values round trip")
    {
        Derived obj;
        // defaults: b1_int=1, b2_int=2, d_int=3, b1_float=1.0, b2_float=1.0, d_float=1.0

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 1);
        REQUIRE(result->b2_int == 2);
        REQUIRE(result->d_int == 3);
    }
}
