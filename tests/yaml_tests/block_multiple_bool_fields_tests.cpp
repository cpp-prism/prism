#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_bool in nested structures across multiple levels round trip", "[yaml][block][bool][nested][struct]")
{
    SECTION("my_bool in root and vec_sp elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;

        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_bool = false;
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_bool = true;
        obj.my_vec_sp.push_back(sub2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_bool == true);
    }

    SECTION("my_bool in root, embedded my_struct, and shared_sub block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = false;
        obj.my_struct.my_bool = true;

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_bool = false;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_bool == false);
    }
}
