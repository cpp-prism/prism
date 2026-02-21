#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format Derived multi-inheritance various combos round trip", "[yaml][block][inheritance][derived][combo]")
{
    SECTION("Derived with all fields non-default block round trip")
    {
        Derived obj;
        obj.b1_int = 100;
        obj.b2_int = 200;
        obj.d_int = 300;
        obj.b1_float = 1.5f;
        obj.b2_float = 2.5f;
        obj.d_float = 3.5f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result->b1_int == 100);
        REQUIRE(result->b2_int == 200);
        REQUIRE(result->d_int == 300);
        REQUIRE(result->b1_float == Catch::Approx(1.5f).epsilon(1e-4));
        REQUIRE(result->b2_float == Catch::Approx(2.5f).epsilon(1e-4));
        REQUIRE(result->d_float == Catch::Approx(3.5f).epsilon(1e-4));
    }

    SECTION("Base2 standalone block round trip")
    {
        Base2 obj;
        obj.b2_int = 77;
        obj.b2_float = 7.7f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Base2>(yaml);

        REQUIRE(result->b2_int == 77);
        REQUIRE(result->b2_float == Catch::Approx(7.7f).epsilon(1e-4));
    }
}
