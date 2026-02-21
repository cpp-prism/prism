#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - level0 deep nesting with all levels populated round trip", "[json][level0][level3][deep]")
{
    SECTION("all 4 levels fully populated round trip")
    {
        level0 obj;
        obj.score = 9.5;
        obj.top.tag = "level1_tag";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "deep_leaf";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == 9.5);
        REQUIRE(result->top.tag == "level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "deep_leaf");
    }

    SECTION("level3 val zero and name empty round trip")
    {
        level0 obj;
        obj.score = 0.0;
        obj.top.tag = "";
        obj.top.middle.x = 0;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == 0.0);
        REQUIRE(result->top.tag == "");
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name == "");
    }

    SECTION("level0 negative score and negative x round trip")
    {
        level0 obj;
        obj.score = -3.14;
        obj.top.tag = "neg";
        obj.top.middle.x = -99;
        obj.top.middle.inner.val = -7;
        obj.top.middle.inner.name = "minus";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->top.middle.x == -99);
        REQUIRE(result->top.middle.inner.val == -7);
        REQUIRE(result->top.middle.inner.name == "minus");
    }
}
