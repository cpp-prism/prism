#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - attributes", "[yaml][attributes]")
{
    SECTION("test yaml alias")
    {
        tst_struct val;
        val.my_int = 111;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("MY_INT1") != std::string::npos);
    }

    SECTION("round trip with yaml alias")
    {
        tst_struct val;
        val.my_int = 111;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->my_int == 111);
    }

    SECTION("my_int field is included in yaml output")
    {
        tst_sub_struct val;
        val.my_int = 1;
        val.my_string = "test";
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("my_int") != std::string::npos);
    }

    SECTION("deserialize from alias key directly")
    {
        std::string yaml = "MY_INT1: 42\nmy_bool: true";
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
    }
}

TEST_CASE("prismYaml - attributes with hybrid format", "[yaml][attributes][hybrid]")
{
    SECTION("hybrid format - field with format flip in all_block")
    {
        tst_sub_struct val;
        val.my_int = 123;
        val.my_bool = true;
        val.my_string = "test";
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        REQUIRE(yaml.find("my_int:") != std::string::npos);
    }

    SECTION("deserialize from flow format")
    {
        std::string yaml = "{\"my_int\": 456, \"my_bool\": false, \"my_string\": \"deserialize\"}";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "deserialize");
    }

    SECTION("deserialize from block format")
    {
        std::string yaml = "my_int: 456\nmy_bool: false\nmy_string: \"deserialize\"";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "deserialize");
    }
}
