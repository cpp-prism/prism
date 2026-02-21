#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format Derived and Base classes scalar field combinations round trip", "[yaml][block][inheritance][scalar]")
{
    SECTION("Derived with all base and derived fields block round trip")
    {
        Derived obj;
        obj.b1_int = 11;
        obj.b1_float = 1.1f;
        obj.b2_int = 22;
        obj.b2_float = 2.2f;
        obj.d_int = 33;
        obj.d_float = 3.3f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result->b1_int == 11);
        REQUIRE(result->b2_int == 22);
        REQUIRE(result->d_int == 33);
    }

    SECTION("Derived with zero values block round trip")
    {
        Derived obj;
        obj.b1_int = 0;
        obj.b1_float = 0.0f;
        obj.b2_int = 0;
        obj.b2_float = 0.0f;
        obj.d_int = 0;
        obj.d_float = 0.0f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);

        REQUIRE(result->b1_int == 0);
        REQUIRE(result->b2_int == 0);
        REQUIRE(result->d_int == 0);
    }

    SECTION("Base1 standalone with negative int block round trip")
    {
        Base1 obj;
        obj.b1_int = -10;
        obj.b1_float = -1.0f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Base1>(yaml);

        REQUIRE(result->b1_int == -10);
    }

    SECTION("Base2 standalone with positive values block round trip")
    {
        Base2 obj;
        obj.b2_int = 500;
        obj.b2_float = 50.5f;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<Base2>(yaml);

        REQUIRE(result->b2_int == 500);
    }
}
