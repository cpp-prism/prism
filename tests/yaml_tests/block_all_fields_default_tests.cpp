#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format default-constructed struct round trip keeps defaults", "[yaml][block][default][round_trip]")
{
    SECTION("tst_sub_struct default values block round trip")
    {
        tst_sub_struct obj;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string.empty());
        REQUIRE(result->my_longlong == 0LL);
    }

    SECTION("level3 default values block round trip")
    {
        level3 obj;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level3>(yaml);

        REQUIRE(result->val == 0);
        REQUIRE(result->name.empty());
    }

    SECTION("Derived default values block round trip")
    {
        Derived obj;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result->b1_int == 1);
        REQUIRE(result->b2_int == 2);
        REQUIRE(result->d_int == 3);
    }
}
