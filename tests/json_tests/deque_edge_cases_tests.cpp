#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_deque_int edge cases round trip", "[json][deque][int]")
{
    SECTION("my_deque_int with negative values round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-300, -100, 0, 100, 300};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -300);
        REQUIRE(result->my_deque_int[1] == -100);
        REQUIRE(result->my_deque_int[2] == 0);
        REQUIRE(result->my_deque_int[3] == 100);
        REQUIRE(result->my_deque_int[4] == 300);
    }

    SECTION("my_deque_int single element round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {42};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 42);
    }

    SECTION("my_deque_int with boundary values round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {INT_MAX, INT_MIN, 0};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == INT_MAX);
        REQUIRE(result->my_deque_int[1] == INT_MIN);
        REQUIRE(result->my_deque_int[2] == 0);
    }

    SECTION("my_deque_int empty round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_deque_int empty by default

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.empty());
    }
}
