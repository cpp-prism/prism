#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - 4-level deep nesting round trip", "[yaml][nesting][struct]")
{
    SECTION("level0->level1->level2->level3 block round trip")
    {
        level0 original;
        original.score = 3.14;
        original.top.tag = "level1_tag";
        original.top.middle.x = 42;
        original.top.middle.inner.val = 99;
        original.top.middle.inner.name = "deepest";

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        REQUIRE(yaml.find("level1_tag") != std::string::npos);
        REQUIRE(yaml.find("deepest") != std::string::npos);

        auto result = prism::yaml::fromYamlString<level0>(yaml);
        REQUIRE(std::abs(result->score - 3.14) < 0.001);
        REQUIRE(result->top.tag == "level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 99);
        REQUIRE(result->top.middle.inner.name == "deepest");
    }

    SECTION("level0->level1->level2->level3 flow round trip")
    {
        level0 original;
        original.score = 1.5;
        original.top.tag = "flow_tag";
        original.top.middle.x = 7;
        original.top.middle.inner.val = 13;
        original.top.middle.inner.name = "flow_deep";

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(std::abs(result->score - 1.5) < 0.001);
        REQUIRE(result->top.tag == "flow_tag");
        REQUIRE(result->top.middle.x == 7);
        REQUIRE(result->top.middle.inner.val == 13);
        REQUIRE(result->top.middle.inner.name == "flow_deep");
    }

    SECTION("default values preserved in block round trip")
    {
        level0 original;
        // all fields at default values

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == 0.0);
        REQUIRE(result->top.tag == "");
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name == "");
    }
}
