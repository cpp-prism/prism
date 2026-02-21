#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - flow format 4-level deep nesting round trip", "[yaml][flow][deep][nesting]")
{
    SECTION("level0->level1->level2->level3 flow round trip")
    {
        level0 obj;
        obj.score = 9.9;
        obj.top.tag = "flow_l1";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "flow_deepest";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(9.9));
        REQUIRE(result->top.tag == "flow_l1");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "flow_deepest");
    }

    SECTION("level0 with default values flow round trip")
    {
        level0 obj;
        // All defaults: score=0, tag="", x=0, val=0, name=""

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == 0.0);
        REQUIRE(result->top.tag.empty());
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name.empty());
    }
}
