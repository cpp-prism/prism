#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format inheritance round trip", "[yaml][flow][inheritance]")
{
    SECTION("Derived class flow format round trip includes all base fields")
    {
        Derived original;
        original.b1_int = 10;
        original.b1_float = 1.25f;
        original.b2_int = 20;
        original.b2_float = 2.25f;
        original.d_int = 30;
        original.d_float = 3.25f;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 10);
        REQUIRE(result->b2_int == 20);
        REQUIRE(result->d_int == 30);
    }

    SECTION("Derived class with default values flow round trip")
    {
        Derived original;
        // b1_int=1, b2_int=2, d_int=3 by default

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 1);
        REQUIRE(result->b2_int == 2);
        REQUIRE(result->d_int == 3);
    }
}
