#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_struct (embedded tst_sub_struct) with list fields round trip", "[json][my_struct][list][combo]")
{
    SECTION("my_struct with vec_sp populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 10;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "inner_str";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 10);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "inner_str");
    }

    SECTION("my_struct with opt_int set round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 20;
        obj.my_struct.my_opt_int = 99;
        obj.my_struct.my_longlong = 12345LL;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 20);
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 99);
        REQUIRE(result->my_struct.my_longlong == 12345LL);
    }

    SECTION("my_struct alongside vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 30;
        obj.my_struct.my_float = 3.3f;

        tst_sub_struct sub;
        sub.my_int = 50;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 30);
        REQUIRE(result->my_vec_sp[0].my_int == 50);
    }
}
