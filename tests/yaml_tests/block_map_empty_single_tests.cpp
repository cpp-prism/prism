#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map empty and single entry round trip", "[yaml][block][map][empty][single]")
{
    SECTION("my_map empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.empty());
    }

    SECTION("my_map single entry with minimal child block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("solo", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("solo").my_int == 5);
    }

    SECTION("my_unordermap single entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct sub;
        sub.my_int = 99;
        obj.my_unordermap["k"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap.at("k").my_int == 99);
    }
}
