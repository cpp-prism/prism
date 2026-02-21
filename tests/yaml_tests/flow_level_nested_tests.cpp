#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - flow format nested structs level1 and level2 with different field combos round trip", "[yaml][flow][nested][level][combo]")
{
    SECTION("level1 with all fields populated flow round trip")
    {
        level1 obj;
        obj.tag = "flow_tag_1";
        obj.middle.x = 42;
        obj.middle.inner.val = 100;
        obj.middle.inner.name = "flow_inner_1";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level1>(yaml);

        REQUIRE(result->tag == "flow_tag_1");
        REQUIRE(result->middle.x == 42);
        REQUIRE(result->middle.inner.val == 100);
        REQUIRE(result->middle.inner.name == "flow_inner_1");
    }

    SECTION("level2 with negative x flow round trip")
    {
        level2 obj;
        obj.x = -55;
        obj.inner.val = -99;
        obj.inner.name = "negative_flow";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level2>(yaml);

        REQUIRE(result->x == -55);
        REQUIRE(result->inner.val == -99);
        REQUIRE(result->inner.name == "negative_flow");
    }

    SECTION("level0 all zero fields flow round trip")
    {
        level0 obj;
        obj.score = 0.0;
        obj.top.tag = "";
        obj.top.middle.x = 0;
        obj.top.middle.inner.val = 0;
        obj.top.middle.inner.name = "";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(0.0).epsilon(1e-9));
        REQUIRE(result->top.tag.empty());
        REQUIRE(result->top.middle.x == 0);
        REQUIRE(result->top.middle.inner.val == 0);
        REQUIRE(result->top.middle.inner.name.empty());
    }
}
