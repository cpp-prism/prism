#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_list_int with boundary and edge values round trip", "[json][list][int][boundary]")
{
    SECTION("my_list_int with INT_MAX and INT_MIN round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {INT_MAX, INT_MIN, 0, -1, 1};
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 5);
        auto it = result->my_list_int.begin();
        REQUIRE(*it == INT_MAX); ++it;
        REQUIRE(*it == INT_MIN); ++it;
        REQUIRE(*it == 0); ++it;
        REQUIRE(*it == -1); ++it;
        REQUIRE(*it == 1);
    }

    SECTION("my_list_int single element round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int = {42};
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 1);
        REQUIRE(result->my_list_int.front() == 42);
    }

    SECTION("my_list_int large list round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 20; ++i)
            obj.my_list_int.push_back(i * 3 - 30);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 20);
        int expected = -30;
        for (auto v : result->my_list_int) {
            REQUIRE(v == expected);
            expected += 3;
        }
    }
}
