#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - nested structs level1 and level2 with different field combos round trip", "[json][nested][level][combo]")
{
    SECTION("level1 with all fields populated round trip")
    {
        level1 obj;
        obj.tag = "test_tag_1";
        obj.middle.x = 42;
        obj.middle.inner.val = 100;
        obj.middle.inner.name = "inner_name_1";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level1>(json);

        REQUIRE(result->tag == "test_tag_1");
        REQUIRE(result->middle.x == 42);
        REQUIRE(result->middle.inner.val == 100);
        REQUIRE(result->middle.inner.name == "inner_name_1");
    }

    SECTION("level2 with negative x and empty name round trip")
    {
        level2 obj;
        obj.x = -55;
        obj.inner.val = -99;
        obj.inner.name = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level2>(json);

        REQUIRE(result->x == -55);
        REQUIRE(result->inner.val == -99);
        REQUIRE(result->inner.name.empty());
    }

    SECTION("level0 with all zero and empty fields round trip")
    {
        level0 obj;
        obj.score = 0.0;
        obj.top.tag = "";
        obj.top.middle.x = 0;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(0.0).epsilon(1e-9));
        REQUIRE(result->top.tag.empty());
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name.empty());
    }
}
