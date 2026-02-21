#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp (list<tst_struct>) round trip", "[yaml][block][list][struct]")
{
    SECTION("my_list_sp with 2 tst_struct elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem1;
        elem1.my_int = 10;
        elem1.my_bool = true;
        elem1.my_string = "block_list_elem1";
        elem1.my_list_int.clear();
        elem1.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(elem1));

        tst_struct elem2;
        elem2.my_int = 20;
        elem2.my_bool = false;
        elem2.my_string = "block_list_elem2";
        elem2.my_list_int.clear();
        elem2.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(elem2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->my_bool == true);
        REQUIRE(it->my_string == "block_list_elem1");
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_bool == false);
        REQUIRE(it->my_string == "block_list_elem2");
    }

    SECTION("my_list_sp empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_list_sp.empty());
    }

    SECTION("my_list_sp element with nested my_vec_sp block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 99;
        elem.my_list_int.clear();
        elem.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 77;
        sub.my_string = "block_nested_in_list";
        elem.my_vec_sp.push_back(sub);

        obj.my_list_sp.push_back(std::move(elem));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 99);
        REQUIRE(result->my_list_sp.front().my_vec_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_vec_sp[0].my_int == 77);
        REQUIRE(result->my_list_sp.front().my_vec_sp[0].my_string == "block_nested_in_list");
    }
}
