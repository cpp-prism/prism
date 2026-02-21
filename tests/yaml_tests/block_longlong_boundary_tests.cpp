#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_longlong boundary values round trip", "[yaml][block][longlong][boundary]")
{
    SECTION("my_longlong max value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_longlong = LLONG_MAX;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == LLONG_MAX);
    }

    SECTION("my_longlong min value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_longlong = LLONG_MIN;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == LLONG_MIN);
    }

    SECTION("my_longlong large negative value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_longlong = -9876543210987654LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_longlong == -9876543210987654LL);
    }
}
