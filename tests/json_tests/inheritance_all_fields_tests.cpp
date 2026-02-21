#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - Derived (multi-inheritance) all fields round trip", "[json][inheritance][combo]")
{
    SECTION("Derived with all fields set to non-default values round trip")
    {
        Derived obj;
        obj.b1_int = 10;
        obj.b1_float = 1.5f;
        obj.b2_int = 20;
        obj.b2_float = 2.5f;
        obj.d_int = 30;
        obj.d_float = 3.5f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 10);
        REQUIRE(result->b1_float == Catch::Approx(1.5f).epsilon(1e-5));
        REQUIRE(result->b2_int == 20);
        REQUIRE(result->b2_float == Catch::Approx(2.5f).epsilon(1e-5));
        REQUIRE(result->d_int == 30);
        REQUIRE(result->d_float == Catch::Approx(3.5f).epsilon(1e-5));
    }

    SECTION("Derived with negative int and float values round trip")
    {
        Derived obj;
        obj.b1_int = -1;
        obj.b1_float = -1.5f;
        obj.b2_int = -2;
        obj.b2_float = -2.5f;
        obj.d_int = -3;
        obj.d_float = -3.5f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == -1);
        REQUIRE(result->b2_int == -2);
        REQUIRE(result->d_int == -3);
        REQUIRE(result->d_float == Catch::Approx(-3.5f).epsilon(1e-5));
    }

    SECTION("Derived with zero values round trip")
    {
        Derived obj;
        obj.b1_int = 0;
        obj.b1_float = 0.0f;
        obj.b2_int = 0;
        obj.b2_float = 0.0f;
        obj.d_int = 0;
        obj.d_float = 0.0f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 0);
        REQUIRE(result->b2_int == 0);
        REQUIRE(result->d_int == 0);
        REQUIRE(result->b1_float == Catch::Approx(0.0f).epsilon(1e-5));
    }
}
