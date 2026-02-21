#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp elements with my_string field variations round trip", "[yaml][block][vec][string][field]")
{
    SECTION("my_vec_sp element with non-empty string block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "block_element_string";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_string == "block_element_string");
    }

    SECTION("my_vec_sp multiple elements with different strings block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "first_block";
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "unicode_block_日本語";
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_string == "first_block");
        REQUIRE(result->my_vec_sp[1].my_string == "unicode_block_日本語");
    }
}
