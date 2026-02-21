#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format float and double boundary values round trip", "[yaml][block][float][double][boundary]")
{
    SECTION("my_double negative small value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_double = -0.0001234;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_double == Catch::Approx(-0.0001234).epsilon(1e-9));
    }

    SECTION("my_float and my_double zero block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_float == Catch::Approx(0.0f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(0.0).epsilon(1e-9));
    }

    SECTION("my_float 1.0f and my_double 1.0 block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_float = 1.0f;
        obj.my_double = 1.0;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_float == Catch::Approx(1.0f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(1.0).epsilon(1e-9));
    }
}
