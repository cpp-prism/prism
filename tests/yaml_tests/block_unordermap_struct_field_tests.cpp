#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format unordered_map<string,struct> as struct field", "[yaml][block][unordered_map][struct]")
{
    SECTION("tst_struct my_unordermap two entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        s1.my_string = "first";
        obj.my_unordermap["alpha"] = s1;

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        s2.my_string = "second";
        obj.my_unordermap["beta"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["alpha"].my_int == 10);
        REQUIRE(result->my_unordermap["alpha"].my_bool == true);
        REQUIRE(result->my_unordermap["alpha"].my_string == "first");
        REQUIRE(result->my_unordermap["beta"].my_int == 20);
        REQUIRE(result->my_unordermap["beta"].my_bool == false);
        REQUIRE(result->my_unordermap["beta"].my_string == "second");
    }

    SECTION("tst_struct my_unordermap empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        // my_unordermap is empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_unordermap.empty());
    }

    SECTION("tst_struct my_unordermap single entry with sub-vec block round trip")
    {
        tst_struct obj;
        obj.my_int = 9;

        tst_sub_struct sub;
        sub.my_int = 77;
        sub.my_string = "sub_with_vec";
        // Also test that sub-struct's my_vec_sp serializes correctly
        tst_sub_struct grandchild;
        grandchild.my_int = 99;
        grandchild.my_string = "gc";
        sub.my_vec_sp.push_back(grandchild);
        obj.my_unordermap["key1"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 9);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["key1"].my_int == 77);
        REQUIRE(result->my_unordermap["key1"].my_string == "sub_with_vec");
        REQUIRE(result->my_unordermap["key1"].my_vec_sp.size() == 1);
        REQUIRE(result->my_unordermap["key1"].my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_unordermap["key1"].my_vec_sp[0].my_string == "gc");
    }
}
