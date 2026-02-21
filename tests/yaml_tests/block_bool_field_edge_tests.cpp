#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_bool edge cases in tst_struct and tst_sub_struct round trip", "[yaml][block][bool][edge]")
{
    SECTION("tst_struct my_bool true with string and float block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_bool = true;
        obj.my_string = "yaml_bool_true";
        obj.my_float = 1.5f;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "yaml_bool_true");
        REQUIRE(result->my_int == 1);
    }

    SECTION("tst_struct my_bool false with opt_int set block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_bool = false;
        obj.my_opt_int = 99;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 99);
    }

    SECTION("tst_sub_struct my_bool true, opt_int negative, longlong negative block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_bool = true;
        obj.my_opt_int = -5;
        obj.my_longlong = -8888LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(*result->my_opt_int == -5);
        REQUIRE(result->my_longlong == -8888LL);
    }
}
