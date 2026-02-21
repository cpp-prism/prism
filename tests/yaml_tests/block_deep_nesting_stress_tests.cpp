#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format deep nesting stress test with many fields set round trip", "[yaml][block][deep][stress][nesting]")
{
    SECTION("level0 with all levels set to extreme values block round trip")
    {
        level0 obj;
        obj.score = 1234567.89;
        obj.top.tag = "stress_test_tag_block";
        obj.top.middle.x = 999999;
        obj.top.middle.inner.val = -888888;
        obj.top.middle.inner.name = "deep_block_name_xyz";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(1234567.89).epsilon(1e-6));
        REQUIRE(result->top.tag == "stress_test_tag_block");
        REQUIRE(result->top.middle.x == 999999);
        REQUIRE(result->top.middle.inner.val == -888888);
        REQUIRE(result->top.middle.inner.name == "deep_block_name_xyz");
    }

    SECTION("level0 with unicode at all levels block round trip")
    {
        level0 obj;
        obj.score = -9876.54321;
        obj.top.tag = "ブロック_block";
        obj.top.middle.x = -12345;
        obj.top.middle.inner.val = 99999;
        obj.top.middle.inner.name = "блок_深度";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(-9876.54321).epsilon(1e-6));
        REQUIRE(result->top.tag == "ブロック_block");
        REQUIRE(result->top.middle.x == -12345);
        REQUIRE(result->top.middle.inner.val == 99999);
        REQUIRE(result->top.middle.inner.name == "блок_深度");
    }
}
