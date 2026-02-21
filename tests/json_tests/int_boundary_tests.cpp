#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - int boundary values round trip", "[json][int][boundary]")
{
    SECTION("my_int max value round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MAX;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == INT_MAX);
    }

    SECTION("my_int min value round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MIN;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == INT_MIN);
    }

    SECTION("my_int zero round trip")
    {
        tst_struct obj;
        obj.my_int = 0;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 0);
    }
}
