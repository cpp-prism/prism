#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map with my_deque_int and my_set_str combo round trip", "[json][map][deque][set][combo]")
{
    SECTION("my_map entry with deque and set in child round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"aaa", "bbb"};

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_deque_int = {10, 20};
        child.my_set_str = {"x", "y"};
        obj.my_map.emplace("child", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("child").my_deque_int.size() == 2);
        REQUIRE(result->my_map.at("child").my_set_str.size() == 2);
    }

    SECTION("my_deque_int and my_set_str with multiple list combos round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int = {1, 2};
        obj.my_list_std_string = {"a", "b"};
        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"p", "q", "r"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 2);
        REQUIRE(result->my_list_std_string.size() == 2);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("p") == 1);
        REQUIRE(result->my_set_str.count("q") == 1);
        REQUIRE(result->my_set_str.count("r") == 1);
    }
}
