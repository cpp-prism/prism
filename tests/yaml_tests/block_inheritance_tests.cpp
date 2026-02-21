#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format inheritance round trip", "[yaml][block][inheritance]")
{
    SECTION("Derived class block format round trip includes all base fields")
    {
        Derived original;
        original.b1_int = 10;
        original.b1_float = 1.25f;
        original.b2_int = 20;
        original.b2_float = 2.25f;
        original.d_int = 30;
        original.d_float = 3.25f;

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 10);
        REQUIRE(std::abs(result->b1_float - 1.25f) < 0.001f);
        REQUIRE(result->b2_int == 20);
        REQUIRE(std::abs(result->b2_float - 2.25f) < 0.001f);
        REQUIRE(result->d_int == 30);
        REQUIRE(std::abs(result->d_float - 3.25f) < 0.001f);
    }

    SECTION("Derived class block format preserves default values")
    {
        Derived original;
        // b1_int=1, b1_float=1.0, b2_int=2, b2_float=1.0, d_int=3, d_float=1.0 (defaults)

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 1);
        REQUIRE(result->b2_int == 2);
        REQUIRE(result->d_int == 3);
    }
}
