#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

// 全局作用域定义结构体
struct CharStruct {
    char c;
    unsigned char uc;
    short s;
    unsigned short us;
};
PRISM_FIELDS(CharStruct, c, uc, s, us)

TEST_CASE("prismJson - char types serialization", "[json][char]")
{
    SECTION("serialize char as integer")
    {
        char val = 'A';
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "65");
    }

    SECTION("deserialize char from json")
    {
        std::string json = "66";
        auto result = prism::json::fromJsonString<char>(json);
        REQUIRE(*result == 'B');
    }

    SECTION("round trip char")
    {
        char original = 'Z';
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<char>(json);
        REQUIRE(*result == original);
    }

    SECTION("serialize unsigned char as integer")
    {
        unsigned char val = 200;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "200");
    }

    SECTION("deserialize unsigned char from json")
    {
        std::string json = "255";
        auto result = prism::json::fromJsonString<unsigned char>(json);
        REQUIRE(*result == 255);
    }

    SECTION("round trip unsigned char")
    {
        unsigned char original = 128;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<unsigned char>(json);
        REQUIRE(*result == original);
    }

    SECTION("serialize short as integer")
    {
        short val = -12345;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "-12345");
    }

    SECTION("deserialize short from json")
    {
        std::string json = "32767";
        auto result = prism::json::fromJsonString<short>(json);
        REQUIRE(*result == 32767);
    }

    SECTION("round trip short")
    {
        short original = -1000;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<short>(json);
        REQUIRE(*result == original);
    }

    SECTION("serialize unsigned short")
    {
        unsigned short val = 65535;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "65535");
    }

    SECTION("serialize char in struct")
    {
        CharStruct obj = {'X', 200, -500, 1000};
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<CharStruct>(json);
        REQUIRE(result->c == 'X');
        REQUIRE(result->uc == 200);
        REQUIRE(result->s == -500);
        REQUIRE(result->us == 1000);
    }
}
