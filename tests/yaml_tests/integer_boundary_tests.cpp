#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>
#include <cstdint>

TEST_CASE("prismYaml - integer boundary values", "[yaml][integer][boundary]")
{
    SECTION("INT_MAX round trip")
    {
        int val = INT_MAX;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == INT_MAX);
    }

    SECTION("INT_MIN round trip")
    {
        int val = INT_MIN;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == INT_MIN);
    }

    SECTION("LLONG_MAX round trip")
    {
        int64_t val = LLONG_MAX;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<int64_t>(yaml);
        REQUIRE(*result == LLONG_MAX);
    }

    SECTION("LLONG_MIN round trip")
    {
        int64_t val = LLONG_MIN;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<int64_t>(yaml);
        REQUIRE(*result == LLONG_MIN);
    }

    SECTION("int64 boundary in struct field")
    {
        tst_sub_struct obj;
        obj.my_longlong = LLONG_MIN;
        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_longlong == LLONG_MIN);
    }

    SECTION("zero value round trip")
    {
        int val = 0;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == 0);
    }
}
