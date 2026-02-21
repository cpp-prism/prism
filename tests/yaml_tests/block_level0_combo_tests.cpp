#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format level0 4-level struct various combos round trip", "[yaml][block][level0][nested]")
{
    SECTION("level0 all fields non-default block round trip")
    {
        level0 obj;
        obj.score = 9.99;
        obj.top.tag = "block_level1_tag";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "block_deepest";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(9.99).epsilon(1e-9));
        REQUIRE(result->top.tag == "block_level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "block_deepest");
    }

    SECTION("level0 with negative score and empty strings block round trip")
    {
        level0 obj;
        obj.score = -2.5;
        obj.top.tag = "";
        obj.top.middle.x = -77;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(-2.5).epsilon(1e-9));
        REQUIRE(result->top.tag.empty());
        REQUIRE(result->top.middle.x == -77);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name.empty());
    }

    SECTION("level3 standalone block round trip")
    {
        level3 obj;
        obj.val = 456;
        obj.name = "block_level3_test";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level3>(yaml);

        REQUIRE(result->val == 456);
        REQUIRE(result->name == "block_level3_test");
    }
}
