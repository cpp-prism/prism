#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - Derived class with newer-style scenarios round trip", "[json][inheritance][derived]")
{
    SECTION("Derived class all base and derived fields round trip")
    {
        Derived original;
        original.b1_int = 100;
        original.b1_float = 1.5f;
        original.b2_int = 200;
        original.b2_float = 2.5f;
        original.d_int = 300;
        original.d_float = 3.5f;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 100);
        REQUIRE(result->b2_int == 200);
        REQUIRE(result->d_int == 300);
    }

    SECTION("Derived class with negative field values round trip")
    {
        Derived original;
        original.b1_int = -10;
        original.b1_float = -1.5f;
        original.b2_int = -20;
        original.b2_float = -2.5f;
        original.d_int = -30;
        original.d_float = -3.5f;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == -10);
        REQUIRE(result->b2_int == -20);
        REQUIRE(result->d_int == -30);
    }

    SECTION("Derived class default values round trip")
    {
        Derived original;
        // b1_int=1, b2_int=2, d_int=3 by default

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 1);
        REQUIRE(result->b2_int == 2);
        REQUIRE(result->d_int == 3);
    }
}
