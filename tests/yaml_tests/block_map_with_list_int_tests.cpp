#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map entries with my_list_int populated round trip", "[yaml][block][map][list_int][combo]")
{
    SECTION("my_map entry with list_int populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 10;
        child.my_list_int = {100, 200, 300};
        child.my_list_std_string.clear();
        obj.my_map.emplace("entry", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("entry").my_int == 10);
        REQUIRE(result->my_map.at("entry").my_list_int.size() == 3);
        auto it = result->my_map.at("entry").my_list_int.begin();
        REQUIRE(*it == 100); ++it;
        REQUIRE(*it == 200); ++it;
        REQUIRE(*it == 300);
    }

    SECTION("my_map two entries each with list_int block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct c1;
        c1.my_int = 1;
        c1.my_list_int = {10, 20};
        c1.my_list_std_string.clear();
        obj.my_map.emplace("a", std::move(c1));

        tst_struct c2;
        c2.my_int = 2;
        c2.my_list_int = {30, 40, 50};
        c2.my_list_std_string.clear();
        obj.my_map.emplace("b", std::move(c2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.at("a").my_list_int.size() == 2);
        REQUIRE(result->my_map.at("b").my_list_int.size() == 3);
    }
}
