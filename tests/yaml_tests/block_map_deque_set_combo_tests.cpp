#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map with my_deque_int and my_set_str combo round trip", "[yaml][block][map][deque][set][combo]")
{
    SECTION("my_deque_int and my_set_str with list combos block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {10, 20};
        obj.my_list_std_string = {"x", "y"};
        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"alpha", "beta"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 2);
        REQUIRE(result->my_list_std_string.size() == 2);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("alpha") == 1);
        REQUIRE(result->my_set_str.count("beta") == 1);
    }

    SECTION("my_map entry with deque and set block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_deque_int = {5, 10};
        obj.my_set_str = {"m", "n"};

        tst_struct child;
        child.my_int = 50;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_deque_int = {1, 2, 3};
        child.my_set_str = {"a", "b", "c"};
        obj.my_map.emplace("sub", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("sub").my_deque_int.size() == 3);
        REQUIRE(result->my_map.at("sub").my_set_str.size() == 3);
    }
}
