#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_opt_int combined with other fields round trip", "[yaml][block][optional][int][combo]")
{
    SECTION("my_opt_int=0 with my_vec_sp populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = 0;

        tst_sub_struct sub;
        sub.my_int = 77;
        sub.my_string = "block_opt_zero";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 0);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 77);
    }

    SECTION("my_opt_int negative with my_deque_int block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = -99;
        obj.my_deque_int = {10, 20, 30};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == -99);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[2] == 30);
    }

    SECTION("my_opt_int nullopt with my_set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = std::nullopt;
        obj.my_set_str = {"x", "y", "z"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("x") == 1);
    }
}
