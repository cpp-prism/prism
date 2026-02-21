#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map with nested tst_struct values round trip", "[yaml][block][map][nested][struct]")
{
    SECTION("my_map entry with vec_sp populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 42;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 7;
        sub.my_longlong = 100LL;
        child.my_vec_sp.push_back(sub);
        obj.my_map.emplace("child1", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("child1").my_int == 42);
        REQUIRE(result->my_map.at("child1").my_vec_sp.size() == 1);
        REQUIRE(result->my_map.at("child1").my_vec_sp[0].my_int == 7);
        REQUIRE(result->my_map.at("child1").my_vec_sp[0].my_longlong == 100LL);
    }

    SECTION("my_map entry with my_deque_int and my_set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_deque_int = {10, 20, 30};
        child.my_set_str = {"x", "y"};
        obj.my_map.emplace("data", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.at("data").my_int == 99);
        REQUIRE(result->my_map.at("data").my_deque_int.size() == 3);
        REQUIRE(result->my_map.at("data").my_set_str.size() == 2);
    }
}
