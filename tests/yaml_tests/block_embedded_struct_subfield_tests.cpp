#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_struct (embedded tst_sub_struct) sub-field round trip", "[yaml][block][struct][embedded][subfield]")
{
    SECTION("my_struct.my_opt_str with value block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 10;
        obj.my_struct.my_opt_str = "embedded_opt_block";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_struct.my_int == 10);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "embedded_opt_block");
    }

    SECTION("my_struct.my_opt_int = 0 (falsy value) block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 20;
        obj.my_struct.my_opt_int = 0;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 20);
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 0);
    }

    SECTION("my_struct.my_vec_sp with one element block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 30;

        tst_sub_struct sub;
        sub.my_int = 99;
        sub.my_bool = true;
        sub.my_string = "block_sub_in_embedded";
        obj.my_struct.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 30);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_struct.my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_struct.my_vec_sp[0].my_string == "block_sub_in_embedded");
    }
}
