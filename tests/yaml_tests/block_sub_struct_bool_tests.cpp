#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_sub_struct my_bool field round trip", "[yaml][block][sub_struct][bool]")
{
    SECTION("tst_sub_struct my_bool true block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_bool = true;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_bool == true);
    }

    SECTION("tst_sub_struct my_bool false block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_bool = false;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_bool == false);
    }

    SECTION("tst_sub_struct in vec_sp my_bool block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
    }
}
