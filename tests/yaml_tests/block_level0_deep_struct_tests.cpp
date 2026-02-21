#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format level0 (4-level nested struct) round trip", "[yaml][block][deep][nesting]")
{
    SECTION("level0 all fields populated block round trip")
    {
        level0 obj;
        obj.score = 9.9;
        obj.top.tag = "block_l1";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "block_deepest";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(9.9));
        REQUIRE(result->top.tag == "block_l1");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "block_deepest");
    }

    SECTION("level3 standalone block round trip")
    {
        level3 obj;
        obj.val = 999;
        obj.name = "block_l3_only";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level3>(yaml);

        REQUIRE(result->val == 999);
        REQUIRE(result->name == "block_l3_only");
    }

    SECTION("level2 standalone block round trip")
    {
        level2 obj;
        obj.x = 77;
        obj.inner.val = 88;
        obj.inner.name = "block_l2";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level2>(yaml);

        REQUIRE(result->x == 77);
        REQUIRE(result->inner.val == 88);
        REQUIRE(result->inner.name == "block_l2");
    }
}
