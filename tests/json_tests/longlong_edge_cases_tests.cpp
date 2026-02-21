#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_longlong and sub-struct my_longlong extreme values round trip", "[json][longlong][boundary]")
{
    SECTION("my_longlong = LLONG_MAX round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_longlong = LLONG_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MAX);
    }

    SECTION("my_longlong = LLONG_MIN round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_longlong = LLONG_MIN;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_longlong == LLONG_MIN);
    }

    SECTION("my_longlong = 0 round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_longlong = 0;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_longlong == 0);
    }

    SECTION("my_struct.my_longlong extreme value round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_longlong = LLONG_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_longlong == LLONG_MAX);
    }

    SECTION("my_vec_sp element with large longlong round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 1;
        sub.my_longlong = 9999999999999LL;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_longlong == 9999999999999LL);
    }
}
