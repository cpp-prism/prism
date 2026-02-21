#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_deque_int and my_set_str edge cases round trip", "[json][deque][set][string]")
{
    SECTION("my_deque_int with negative and zero values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-10, 0, 10, -999, 42};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -10);
        REQUIRE(result->my_deque_int[1] == 0);
        REQUIRE(result->my_deque_int[2] == 10);
        REQUIRE(result->my_deque_int[3] == -999);
        REQUIRE(result->my_deque_int[4] == 42);
    }

    SECTION("my_set_str with sorted unique strings round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"apple", "banana", "cherry", "date"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 4);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("banana") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
        REQUIRE(result->my_set_str.count("date") == 1);
    }

    SECTION("my_deque_int and my_set_str together round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"x", "y", "z"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("x") == 1);
        REQUIRE(result->my_set_str.count("y") == 1);
        REQUIRE(result->my_set_str.count("z") == 1);
    }
}
