#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map entries with my_string field variations round trip", "[yaml][block][map][string][field]")
{
    SECTION("my_map entry with non-empty string in child block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_string = "child_block_string";
        obj.my_map.emplace("node", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("node").my_string == "child_block_string");
    }

    SECTION("root my_string alongside map block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_string = "root_block_str";

        tst_struct child;
        child.my_int = 50;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_string = "nested_block_str";
        obj.my_map.emplace("item", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "root_block_str");
        REQUIRE(result->my_map.at("item").my_string == "nested_block_str");
    }
}
