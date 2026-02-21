#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - tst_sub_struct all non-pointer fields simultaneously round trip", "[json][sub_struct][all][scalar]")
{
    SECTION("tst_sub_struct all fields positive/non-null round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 123;
        obj.my_bool = true;
        obj.my_float = 3.14f;
        obj.my_double = 2.71828;
        obj.my_string = "full_sub_struct";
        obj.my_longlong = 9999999999LL;
        obj.my_opt_str = "sub_opt";
        obj.my_opt_int = 100;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 123);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(3.14f).epsilon(1e-4));
        REQUIRE(result->my_double == Catch::Approx(2.71828).epsilon(1e-9));
        REQUIRE(result->my_string == "full_sub_struct");
        REQUIRE(result->my_longlong == 9999999999LL);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(result->my_opt_str.value() == "sub_opt");
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == 100);
    }

    SECTION("tst_sub_struct all optional fields null round trip")
    {
        tst_sub_struct obj;
        obj.my_int = -1;
        obj.my_bool = false;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;
        obj.my_string = "";
        obj.my_longlong = 0LL;
        obj.my_opt_str = std::nullopt;
        obj.my_opt_int = std::nullopt;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == -1);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string.empty());
        REQUIRE(result->my_longlong == 0LL);
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(!result->my_opt_int.has_value());
    }
}
