#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_bool edge cases in tst_struct and tst_sub_struct round trip", "[json][bool][edge]")
{
    SECTION("tst_struct my_bool true with all scalar fields set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_bool = true;
        obj.my_string = "with_bool_true";
        obj.my_float = 1.5f;
        obj.my_double = 2.5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "with_bool_true");
        REQUIRE(result->my_int == 1);
    }

    SECTION("tst_struct my_bool false with opt_int set round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_bool = false;
        obj.my_opt_int = 42;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 42);
    }

    SECTION("tst_sub_struct my_bool true, opt_int set, longlong negative round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_bool = true;
        obj.my_opt_int = -5;
        obj.my_longlong = -9999LL;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(*result->my_opt_int == -5);
        REQUIRE(result->my_longlong == -9999LL);
    }
}
