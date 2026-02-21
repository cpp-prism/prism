#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - inheritance", "[yaml][inheritance]")
{
    SECTION("serialize Derived class includes base fields")
    {
        Derived d;
        d.b1_int = 10;
        d.b1_float = 1.5f;
        d.b2_int = 20;
        d.b2_float = 2.5f;
        d.d_int = 30;
        d.d_float = 3.5f;

        std::string yaml = prism::yaml::toYamlStringBlock(d, 2);

        // 验证所有基类和派生类字段均出现在 YAML 中
        REQUIRE(yaml.find("b1_int") != std::string::npos);
        REQUIRE(yaml.find("b1_float") != std::string::npos);
        REQUIRE(yaml.find("b2_int") != std::string::npos);
        REQUIRE(yaml.find("b2_float") != std::string::npos);
        REQUIRE(yaml.find("d_int") != std::string::npos);
        REQUIRE(yaml.find("d_float") != std::string::npos);
    }

    SECTION("deserialize Derived class from flow yaml")
    {
        std::string yaml = "{b1_int: 11, b1_float: 1.1, b2_int: 22, b2_float: 2.2, d_int: 33, d_float: 3.3}";
        auto result = prism::yaml::fromYamlString<Derived>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 11);
        REQUIRE(result->b2_int == 22);
        REQUIRE(result->d_int == 33);
    }

    SECTION("round trip Derived class via flow format")
    {
        Derived original;
        original.b1_int = 100;
        original.b1_float = 1.25f;
        original.b2_int = 200;
        original.b2_float = 2.25f;
        original.d_int = 300;
        original.d_float = 3.25f;

        // 使用 flow 格式保证 round trip 正确性
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<Derived>(yaml);
        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == original.b1_int);
        REQUIRE(result->b2_int == original.b2_int);
        REQUIRE(result->d_int == original.d_int);
        REQUIRE(result->b1_float == original.b1_float);
        REQUIRE(result->b2_float == original.b2_float);
        REQUIRE(result->d_float == original.d_float);
    }
}
