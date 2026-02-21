#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - flow format level0 deep nested struct round trip", "[yaml][flow][deep][nested][struct]")
{
    SECTION("level0 all fields non-default flow round trip")
    {
        level0 obj;
        obj.score = 7.77;
        obj.top.tag = "flow_level1_tag";
        obj.top.middle.x = 33;
        obj.top.middle.inner.val = 55;
        obj.top.middle.inner.name = "flow_deepest";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level0>(yaml);

        REQUIRE(result->score == Catch::Approx(7.77).epsilon(1e-9));
        REQUIRE(result->top.tag == "flow_level1_tag");
        REQUIRE(result->top.middle.x == 33);
        REQUIRE(result->top.middle.inner.val == 55);
        REQUIRE(result->top.middle.inner.name == "flow_deepest");
    }

    SECTION("level2 standalone flow round trip")
    {
        level2 obj;
        obj.x = 44;
        obj.inner.val = 66;
        obj.inner.name = "flow_inner_name";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<level2>(yaml);

        REQUIRE(result->x == 44);
        REQUIRE(result->inner.val == 66);
        REQUIRE(result->inner.name == "flow_inner_name");
    }
}
