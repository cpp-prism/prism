#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - 4-level deep nesting round trip", "[json][nesting][struct]")
{
    SECTION("level0->level1->level2->level3 round trip")
    {
        level0 original;
        original.score = 3.14;
        original.top.tag = "level1_tag";
        original.top.middle.x = 42;
        original.top.middle.inner.val = 99;
        original.top.middle.inner.name = "deepest";

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("level1_tag") != std::string::npos);
        REQUIRE(json.find("deepest") != std::string::npos);

        auto result = prism::json::fromJsonString<level0>(json);
        REQUIRE(result->score == 3.14);
        REQUIRE(result->top.tag == "level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 99);
        REQUIRE(result->top.middle.inner.name == "deepest");
    }

    SECTION("default values preserved in round trip")
    {
        level0 original;
        // all fields at default values
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<level0>(json);
        REQUIRE(result->score == 0.0);
        REQUIRE(result->top.tag == "");
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name == "");
    }
}
