#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_int, my_deque_int, and my_set_str all populated round trip", "[yaml][block][list][deque][set][combo]")
{
    SECTION("all three containers populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {10, 20, 30};
        obj.my_list_std_string = {"a", "b", "c"};
        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"x", "y", "z"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        auto lit = result->my_list_int.begin();
        REQUIRE(*lit == 10); ++lit;
        REQUIRE(*lit == 20);

        REQUIRE(result->my_list_std_string.size() == 3);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 100);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("z") == 1);
    }

    SECTION("empty list alongside deque and set block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-5, 5};
        obj.my_set_str = {"single_val"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_deque_int[0] == -5);
        REQUIRE(result->my_set_str.count("single_val") == 1);
    }
}
