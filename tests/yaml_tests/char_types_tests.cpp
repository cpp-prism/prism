#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

// 全局作用域定义结构体
struct CharStruct {
    char c;
    unsigned char uc;
    short s;
    unsigned short us;
};
PRISM_FIELDS(CharStruct, c, uc, s, us)

TEST_CASE("prismYaml - char types serialization", "[yaml][char]")
{
    SECTION("serialize char as integer")
    {
        char val = 'A';
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "65");
    }

    SECTION("deserialize char from yaml")
    {
        std::string yaml = "66";
        auto result = prism::yaml::fromYamlString<char>(yaml);
        REQUIRE(*result == 'B');
    }

    SECTION("round trip char")
    {
        char original = 'Z';
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<char>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("serialize unsigned char as integer")
    {
        unsigned char val = 200;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "200");
    }

    SECTION("deserialize unsigned char from yaml")
    {
        std::string yaml = "255";
        auto result = prism::yaml::fromYamlString<unsigned char>(yaml);
        REQUIRE(*result == 255);
    }

    SECTION("round trip unsigned char")
    {
        unsigned char original = 128;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<unsigned char>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("serialize short as integer")
    {
        short val = -12345;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "-12345");
    }

    SECTION("deserialize short from yaml")
    {
        std::string yaml = "32767";
        auto result = prism::yaml::fromYamlString<short>(yaml);
        REQUIRE(*result == 32767);
    }

    SECTION("round trip short")
    {
        short original = -1000;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<short>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("serialize unsigned short")
    {
        unsigned short val = 65535;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "65535");
    }

    SECTION("serialize char in struct")
    {
        CharStruct obj = {'X', 200, -500, 1000};
        std::string yaml = prism::yaml::toYamlString(obj);
        auto result = prism::yaml::fromYamlString<CharStruct>(yaml);
        REQUIRE(result->c == 'X');
        REQUIRE(result->uc == 200);
        REQUIRE(result->s == -500);
        REQUIRE(result->us == 1000);
    }
}
