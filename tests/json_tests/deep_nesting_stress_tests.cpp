#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - deep nesting stress test with many fields set round trip", "[json][deep][stress][nesting]")
{
    SECTION("level0 with all levels set to max values round trip")
    {
        level0 obj;
        obj.score = 1234567.89;
        obj.top.tag = "stress_test_tag_abcdefghij";
        obj.top.middle.x = 999999;
        obj.top.middle.inner.val = -888888;
        obj.top.middle.inner.name = "deep_stress_test_name_xyz";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(1234567.89).epsilon(1e-6));
        REQUIRE(result->top.tag == "stress_test_tag_abcdefghij");
        REQUIRE(result->top.middle.x == 999999);
        REQUIRE(result->top.middle.inner.val == -888888);
        REQUIRE(result->top.middle.inner.name == "deep_stress_test_name_xyz");
    }

    SECTION("level0 with unicode at all levels round trip")
    {
        level0 obj;
        obj.score = -9876.54321;
        obj.top.tag = "标签_label";
        obj.top.middle.x = -12345;
        obj.top.middle.inner.val = 99999;
        obj.top.middle.inner.name = "최하위_innermost";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<level0>(json);

        REQUIRE(result->score == Catch::Approx(-9876.54321).epsilon(1e-6));
        REQUIRE(result->top.tag == "标签_label");
        REQUIRE(result->top.middle.x == -12345);
        REQUIRE(result->top.middle.inner.val == 99999);
        REQUIRE(result->top.middle.inner.name == "최하위_innermost");
    }
}
