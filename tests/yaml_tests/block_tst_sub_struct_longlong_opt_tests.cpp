#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format tst_sub_struct my_longlong and my_opt_int combinations round trip", "[yaml][block][sub_struct][longlong][opt_int]")
{
    SECTION("tst_sub_struct max longlong and positive opt_int block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_longlong = LLONG_MAX;
        obj.my_opt_int = INT_MAX;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == LLONG_MAX);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MAX);
    }

    SECTION("tst_sub_struct min longlong and negative opt_int block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_longlong = LLONG_MIN;
        obj.my_opt_int = INT_MIN;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == LLONG_MIN);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == INT_MIN);
    }

    SECTION("tst_sub_struct zero longlong and nullopt block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_longlong = 0LL;
        obj.my_opt_int = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == 0LL);
        REQUIRE(!result->my_opt_int.has_value());
    }
}
