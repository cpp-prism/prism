#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format int boundary values round trip", "[yaml][block][int][boundary]")
{
    SECTION("my_int max value block round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MAX;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == INT_MAX);
    }

    SECTION("my_int min value block round trip")
    {
        tst_struct obj;
        obj.my_int = INT_MIN;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == INT_MIN);
    }

    SECTION("tst_sub_struct my_int max and min block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = INT_MAX;
        obj.my_longlong = static_cast<long long>(INT_MIN);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == INT_MAX);
        REQUIRE(result->my_longlong == static_cast<long long>(INT_MIN));
    }
}
