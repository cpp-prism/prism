#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_int, my_deque_int, and my_set_str all populated round trip", "[json][list][deque][set][combo]")
{
    SECTION("all three containers populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {10, 20, 30};
        obj.my_list_std_string = {"a", "b", "c"};
        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"x", "y", "z"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 3);
        auto lit = result->my_list_int.begin();
        REQUIRE(*lit == 10); ++lit;
        REQUIRE(*lit == 20); ++lit;
        REQUIRE(*lit == 30);

        REQUIRE(result->my_list_std_string.size() == 3);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 100);
        REQUIRE(result->my_deque_int[2] == 300);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("x") == 1);
        REQUIRE(result->my_set_str.count("z") == 1);
    }

    SECTION("large list_int alongside non-empty deque and set round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 20; ++i)
            obj.my_list_int.push_back(i);

        obj.my_deque_int = {-1, 0, 1};
        obj.my_set_str = {"only"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 20);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 1);
        REQUIRE(result->my_set_str.count("only") == 1);
    }

    SECTION("empty list_int with deque and set round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {42};
        obj.my_set_str = {"single"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 42);
        REQUIRE(result->my_set_str.count("single") == 1);
    }
}
