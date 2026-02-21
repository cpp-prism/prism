#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - level0 4-level struct with numeric precision round trip", "[json][level0][nested][precision]")
{
    SECTION("level0 with negative score and unicode string round trip")
    {
        level0 obj;
        obj.score = -0.001;
        obj.top.tag = "中文tag";
        obj.top.middle.x = -100;
        obj.top.middle.inner.val = -50;
        obj.top.middle.inner.name = "深层字段";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(-0.001).epsilon(1e-9));
        REQUIRE(result->top.tag == "中文tag");
        REQUIRE(result->top.middle.x == -100);
        REQUIRE(result->top.middle.inner.val == -50);
        REQUIRE(result->top.middle.inner.name == "深层字段");
    }

    SECTION("level3 standalone round trip")
    {
        level3 obj;
        obj.val = 123;
        obj.name = "level3_test";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level3>(json);

        REQUIRE(result->val == 123);
        REQUIRE(result->name == "level3_test");
    }

    SECTION("level1 standalone round trip")
    {
        level1 obj;
        obj.tag = "l1_tag";
        obj.middle.x = 77;
        obj.middle.inner.val = 88;
        obj.middle.inner.name = "l1_name";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level1>(json);

        REQUIRE(result->tag == "l1_tag");
        REQUIRE(result->middle.x == 77);
        REQUIRE(result->middle.inner.val == 88);
        REQUIRE(result->middle.inner.name == "l1_name");
    }
}
