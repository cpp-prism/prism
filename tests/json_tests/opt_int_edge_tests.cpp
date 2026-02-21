#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_opt_int with boundary and edge values round trip", "[json][optional][int][boundary][edge]")
{
    SECTION("my_opt_int with INT_MAX value round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = INT_MAX;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == INT_MAX);
    }

    SECTION("my_opt_int with INT_MIN value round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = INT_MIN;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == INT_MIN);
    }

    SECTION("my_opt_int in sub_struct with vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_opt_int = 42;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(result->my_vec_sp[0].my_opt_int.value() == 42);
    }
}
