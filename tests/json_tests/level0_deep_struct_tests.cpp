#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - level0 (4-level nested struct) standalone round trip", "[json][deep][nesting][standalone]")
{
    SECTION("level0 all fields populated round trip")
    {
        level0 obj;
        obj.score = 9.9;
        obj.top.tag = "json_l1";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "json_deepest";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == 9.9);
        REQUIRE(result->top.tag == "json_l1");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "json_deepest");
    }

    SECTION("level0 negative score round trip")
    {
        level0 obj;
        obj.score = -100.5;
        obj.top.tag = "negative";
        obj.top.middle.x = -99;
        obj.top.middle.inner.val = -1;
        obj.top.middle.inner.name = "negative_inner";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == -100.5);
        REQUIRE(result->top.tag == "negative");
        REQUIRE(result->top.middle.x == -99);
        REQUIRE(result->top.middle.inner.val == -1);
        REQUIRE(result->top.middle.inner.name == "negative_inner");
    }

    SECTION("level3 standalone round trip")
    {
        level3 obj;
        obj.val = 777;
        obj.name = "level3_standalone";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level3>(json);

        REQUIRE(result->val == 777);
        REQUIRE(result->name == "level3_standalone");
    }
}
