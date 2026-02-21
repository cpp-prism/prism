#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_deque_int and my_set_str edge cases round trip", "[yaml][block][deque][set][string]")
{
    SECTION("my_deque_int with negative and zero values block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-10, 0, 10, -999, 42};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -10);
        REQUIRE(result->my_deque_int[1] == 0);
        REQUIRE(result->my_deque_int[2] == 10);
        REQUIRE(result->my_deque_int[3] == -999);
        REQUIRE(result->my_deque_int[4] == 42);
    }

    SECTION("my_set_str with sorted unique strings block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"apple", "banana", "cherry"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("banana") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
    }

    SECTION("my_deque_int and my_set_str together block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"foo", "bar", "baz"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("foo") == 1);
        REQUIRE(result->my_set_str.count("bar") == 1);
        REQUIRE(result->my_set_str.count("baz") == 1);
    }
}
