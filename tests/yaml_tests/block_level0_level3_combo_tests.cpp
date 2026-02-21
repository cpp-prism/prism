#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format level0 deep nesting with all levels populated round trip", "[yaml][block][level0][level3][deep]")
{
    SECTION("all 4 levels fully populated block round trip")
    {
        level0 obj;
        obj.score = 9.5;
        obj.top.tag = "level1_tag";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "deep_leaf";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == 9.5);
        REQUIRE(result->top.tag == "level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "deep_leaf");
    }

    SECTION("level3 val zero and name empty block round trip")
    {
        level0 obj;
        obj.score = 0.0;
        obj.top.tag = "";
        obj.top.middle.x = 0;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == 0.0);
        REQUIRE(result->top.tag == "");
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name == "");
    }

    SECTION("level0 with non-trivial values at each level block round trip")
    {
        level0 obj;
        obj.score = -1.5;
        obj.top.tag = "root";
        obj.top.middle.x = -50;
        obj.top.middle.inner.val = 255;
        obj.top.middle.inner.name = "leaf_node";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->top.middle.x == -50);
        REQUIRE(result->top.middle.inner.val == 255);
        REQUIRE(result->top.middle.inner.name == "leaf_node");
    }
}
