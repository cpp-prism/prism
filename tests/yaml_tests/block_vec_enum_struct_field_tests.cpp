#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_enum and my_map_enum in tst_struct", "[yaml][block][enum][vector][map]")
{
    SECTION("tst_struct my_vec_enum with values block round trip")
    {
        tst_struct obj;
        obj.my_int = 11;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {english, SimplifiedChinese, TraditionalChinese};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 11);
        REQUIRE(result->my_vec_enum.size() == 3);
        REQUIRE(result->my_vec_enum[0] == english);
        REQUIRE(result->my_vec_enum[1] == SimplifiedChinese);
        REQUIRE(result->my_vec_enum[2] == TraditionalChinese);
    }

    SECTION("tst_struct my_vec_enum empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 12;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_vec_enum is empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 12);
        REQUIRE(result->my_vec_enum.empty());
    }

    SECTION("tst_struct my_map_enum with entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 13;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["lang_a"] = english;
        obj.my_map_enum["lang_b"] = SimplifiedChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 13);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["lang_a"] == english);
        REQUIRE(result->my_map_enum["lang_b"] == SimplifiedChinese);
    }
}
