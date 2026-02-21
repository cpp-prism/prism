#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - struct with map<string, struct> field round trip", "[yaml][struct][map]")
{
    SECTION("tst_struct my_map field serialization")
    {
        tst_struct outer;
        outer.my_int = 1;

        tst_struct inner;
        inner.my_int = 42;
        inner.my_string = "inner_value";
        outer.my_map["child1"] = std::move(inner);

        std::string yaml = prism::yaml::toYamlStringFlow(outer);
        REQUIRE(yaml.find("child1") != std::string::npos);
        REQUIRE(yaml.find("42") != std::string::npos);
        REQUIRE(yaml.find("inner_value") != std::string::npos);
    }

    SECTION("tst_struct my_map field round trip")
    {
        tst_struct outer;
        outer.my_int = 5;

        tst_struct inner1;
        inner1.my_int = 100;
        inner1.my_bool = true;
        outer.my_map["key_a"] = std::move(inner1);

        tst_struct inner2;
        inner2.my_int = 200;
        inner2.my_bool = false;
        outer.my_map["key_b"] = std::move(inner2);

        std::string yaml = prism::yaml::toYamlStringFlow(outer);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["key_a"].my_int == 100);
        REQUIRE(result->my_map["key_a"].my_bool == true);
        REQUIRE(result->my_map["key_b"].my_int == 200);
        REQUIRE(result->my_map["key_b"].my_bool == false);
    }
}
