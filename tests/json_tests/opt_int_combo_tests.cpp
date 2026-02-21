#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismJson - my_opt_int combined with other fields round trip", "[json][optional][int][combo]")
{
    SECTION("my_opt_int=0 with my_vec_sp populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = 0;

        tst_sub_struct sub;
        sub.my_int = 99;
        sub.my_string = "opt_int_zero";
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 0);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 99);
    }

    SECTION("my_opt_int negative with my_deque_int round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = -42;
        obj.my_deque_int = {-1, 0, 1, 100};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == -42);
        REQUIRE(result->my_deque_int.size() == 4);
        REQUIRE(result->my_deque_int[0] == -1);
        REQUIRE(result->my_deque_int[3] == 100);
    }

    SECTION("my_opt_int nullopt with my_set_str round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = std::nullopt;
        obj.my_set_str = {"apple", "banana", "cherry"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
    }

    SECTION("my_opt_int INT_MAX with my_string round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = INT_MAX;
        obj.my_string = "max_int_test";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MAX);
        REQUIRE(result->my_string == "max_int_test");
    }
}
