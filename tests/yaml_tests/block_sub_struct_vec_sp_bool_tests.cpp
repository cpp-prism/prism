#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_sub_struct vec_sp with bool and string fields round trip", "[yaml][block][sub_struct][vec_sp][bool][string]")
{
    SECTION("vec_sp element with bool true and non-empty string block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_bool = true;
        sub.my_string = "hello";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 5);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "hello");
    }

    SECTION("vec_sp two elements mixing bool true and false block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_bool = true;
        s1.my_string = "alpha";
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_bool = false;
        s2.my_string = "beta";
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "alpha");
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "beta");
    }

    SECTION("vec_sp element with empty string and false bool block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 0;
        sub.my_bool = false;
        sub.my_string = "";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_bool == false);
        REQUIRE(result->my_vec_sp[0].my_string == "");
    }
}
