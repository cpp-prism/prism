#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_opt_struct alongside my_vec_sp round trip", "[yaml][block][opt_struct][vec_sp][combo]")
{
    SECTION("opt_struct set and vec_sp one element block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 50;
        obj.my_opt_struct->my_string = "opt_inner";

        tst_sub_struct sub;
        sub.my_int = 100;
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 50);
        REQUIRE(result->my_opt_struct->my_string == "opt_inner");
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
    }

    SECTION("opt_struct nullopt and vec_sp two elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_struct = std::nullopt;

        tst_sub_struct s1;
        s1.my_int = 1;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(!result->my_opt_struct.has_value());
        REQUIRE(result->my_vec_sp.size() == 2);
    }

    SECTION("opt_struct set with bool and vec_sp empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 33;
        obj.my_opt_struct->my_bool = true;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_vec_sp.empty());
    }
}
