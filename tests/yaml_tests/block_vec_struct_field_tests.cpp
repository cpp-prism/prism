#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format vector<struct> as struct field", "[yaml][block][vector][struct]")
{
    SECTION("tst_struct my_vec_sp two elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 7;

        tst_sub_struct e1;
        e1.my_int = 11;
        e1.my_bool = true;
        e1.my_string = "elem_one";
        obj.my_vec_sp.push_back(e1);

        tst_sub_struct e2;
        e2.my_int = 22;
        e2.my_bool = false;
        e2.my_string = "elem_two";
        obj.my_vec_sp.push_back(e2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "elem_one");
        REQUIRE(result->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "elem_two");
    }

    SECTION("tst_struct my_vec_sp single element block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;

        tst_sub_struct e1;
        e1.my_int = 99;
        e1.my_bool = true;
        e1.my_string = "only_elem";
        obj.my_vec_sp.push_back(e1);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "only_elem");
    }

    SECTION("tst_struct my_vec_sp empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        // my_vec_sp is empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_vec_sp.empty());
    }
}
