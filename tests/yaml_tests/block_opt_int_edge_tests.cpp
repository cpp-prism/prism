#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_opt_int with boundary values round trip", "[yaml][block][optional][int][boundary]")
{
    SECTION("my_opt_int with INT_MAX block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = INT_MAX;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == INT_MAX);
    }

    SECTION("my_opt_int with INT_MIN block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = INT_MIN;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == INT_MIN);
    }

    SECTION("my_opt_int nullopt block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(!result->my_opt_int.has_value());
    }
}
