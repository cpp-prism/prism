#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format integer boundary values in struct", "[yaml][block][integer][boundary]")
{
    SECTION("tst_struct my_int = INT_MAX block round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MAX;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == INT_MAX);
    }

    SECTION("tst_struct my_int = INT_MIN block round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MIN;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == INT_MIN);
    }

    SECTION("tst_struct my_longlong = LLONG_MAX block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_longlong = LLONG_MAX;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_longlong == LLONG_MAX);
    }

    SECTION("tst_struct my_longlong = LLONG_MIN block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_longlong = LLONG_MIN;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_longlong == LLONG_MIN);
    }

    SECTION("tst_sub_struct my_opt_int = INT_MAX block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_opt_int = INT_MAX;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MAX);
    }
}
