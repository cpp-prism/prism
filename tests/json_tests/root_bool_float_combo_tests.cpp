#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>

TEST_CASE("prismJson - root level bool and float/double combos round trip", "[json][bool][float][double][combo]")
{
    SECTION("my_bool true with my_float positive and my_double negative round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_float = 3.14f;
        obj.my_double = -2.718;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(3.14f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(-2.718).epsilon(1e-9));
    }

    SECTION("my_bool false with my_float zero and my_double large round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = false;
        obj.my_float = 0.0f;
        obj.my_double = 1.23456789012345;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_float == Catch::Approx(0.0f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(1.23456789012345).epsilon(1e-9));
    }

    SECTION("my_bool true with my_longlong combo round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_longlong = 9876543210LL;
        obj.my_double = 0.001;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_double == Catch::Approx(0.001).epsilon(1e-9));
    }
}
