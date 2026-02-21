#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cfloat>

TEST_CASE("prismJson - float and double boundary values round trip", "[json][float][double][boundary]")
{
    SECTION("my_float max and min boundary values round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_float = FLT_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(FLT_MAX).epsilon(1e-3));
    }

    SECTION("my_double negative small value round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_double = -0.0001234;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_double == Catch::Approx(-0.0001234).epsilon(1e-9));
    }

    SECTION("my_float zero and my_double zero round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(0.0f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(0.0).epsilon(1e-9));
    }
}
