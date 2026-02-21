#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_struct default list values preserved", "[yaml][block][list][default]")
{
    SECTION("tst_struct my_list_int default {111, 2222} preserved in block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        // Do NOT clear my_list_int or my_list_std_string — use defaults

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_int.size() == 2);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == 111);
        REQUIRE(*it++ == 2222);
    }

    SECTION("tst_struct my_list_std_string default {\"111\",\"2222\"} preserved in block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        // Do NOT clear my_list_std_string — use defaults

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_list_std_string.size() == 2);
        auto sit = result->my_list_std_string.begin();
        REQUIRE(*sit++ == "111");
        REQUIRE(*sit++ == "2222");
    }

    SECTION("tst_struct list<struct> elements with default my_list_int values preserved in block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        // Add list_sp elements that have non-empty default my_list_int, my_list_std_string
        tst_struct elem;
        elem.my_int = 99;
        elem.my_bool = true;
        // elem.my_list_int = {111, 2222} by default
        // elem.my_list_std_string = {"111", "2222"} by default
        obj.my_list_sp.push_back(std::move(elem));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        auto& elem_result = result->my_list_sp.front();
        REQUIRE(elem_result.my_int == 99);
        REQUIRE(elem_result.my_bool == true);
        REQUIRE(elem_result.my_list_int.size() == 2);
        auto it = elem_result.my_list_int.begin();
        REQUIRE(*it++ == 111);
        REQUIRE(*it++ == 2222);
        REQUIRE(elem_result.my_list_std_string.size() == 2);
        auto sit = elem_result.my_list_std_string.begin();
        REQUIRE(*sit++ == "111");
        REQUIRE(*sit++ == "2222");
    }
}
