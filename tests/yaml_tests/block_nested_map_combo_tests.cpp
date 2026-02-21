#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map entries each containing other containers round trip", "[yaml][block][map][nested][combo]")
{
    SECTION("my_map child with deque_int and set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 10;
        child.my_deque_int = {1, 2, 3};
        child.my_set_str = {"x", "y"};
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("nested", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.at("nested").my_deque_int.size() == 3);
        REQUIRE(result->my_map.at("nested").my_set_str.count("x") == 1);
    }

    SECTION("my_map child with vec_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 20;
        child.my_vec_enum = {language::english, language::unknow};
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("langs", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.at("langs").my_vec_enum.size() == 2);
        REQUIRE(result->my_map.at("langs").my_vec_enum[0] == language::english);
    }
}
