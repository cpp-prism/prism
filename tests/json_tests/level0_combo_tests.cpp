#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - level0 deep nested struct with different field combos round trip", "[json][deep][nested][struct]")
{
    SECTION("level0 all fields non-default round trip")
    {
        level0 obj;
        obj.score = 9.99;
        obj.top.tag = "level1_tag";
        obj.top.middle.x = 42;
        obj.top.middle.inner.val = 100;
        obj.top.middle.inner.name = "deepest";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(9.99).epsilon(1e-9));
        REQUIRE(result->top.tag == "level1_tag");
        REQUIRE(result->top.middle.x == 42);
        REQUIRE(result->top.middle.inner.val == 100);
        REQUIRE(result->top.middle.inner.name == "deepest");
    }

    SECTION("level0 with negative score and empty strings round trip")
    {
        level0 obj;
        obj.score = -1.5;
        obj.top.tag = "";
        obj.top.middle.x = -99;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(-1.5).epsilon(1e-9));
        REQUIRE(result->top.tag.empty());
        REQUIRE(result->top.middle.x == -99);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name.empty());
    }

    SECTION("level2 standalone with inner round trip")
    {
        level2 obj;
        obj.x = 55;
        obj.inner.val = 77;
        obj.inner.name = "inner_name";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level2>(json);

        REQUIRE(result->x == 55);
        REQUIRE(result->inner.val == 77);
        REQUIRE(result->inner.name == "inner_name");
    }
}
