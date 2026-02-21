#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - Derived multi-inheritance various field combos round trip", "[json][inheritance][derived][combo]")
{
    SECTION("Derived with all base and derived fields set round trip")
    {
        Derived obj;
        obj.b1_int = 100;
        obj.b2_int = 200;
        obj.d_int = 300;
        obj.b1_float = 1.5f;
        obj.b2_float = 2.5f;
        obj.d_float = 3.5f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 100);
        REQUIRE(result->b2_int == 200);
        REQUIRE(result->d_int == 300);
        REQUIRE(result->b1_float == Catch::Approx(1.5f).epsilon(1e-4));
        REQUIRE(result->b2_float == Catch::Approx(2.5f).epsilon(1e-4));
        REQUIRE(result->d_float == Catch::Approx(3.5f).epsilon(1e-4));
    }

    SECTION("Derived with zero float values round trip")
    {
        Derived obj;
        obj.b1_int = 0;
        obj.b2_int = 0;
        obj.d_int = 0;
        obj.b1_float = 0.0f;
        obj.b2_float = 0.0f;
        obj.d_float = 0.0f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 0);
        REQUIRE(result->b2_int == 0);
        REQUIRE(result->d_int == 0);
        REQUIRE(result->b1_float == Catch::Approx(0.0f).epsilon(1e-4));
        REQUIRE(result->b2_float == Catch::Approx(0.0f).epsilon(1e-4));
        REQUIRE(result->d_float == Catch::Approx(0.0f).epsilon(1e-4));
    }

    SECTION("Base1 standalone round trip")
    {
        Base1 obj;
        obj.b1_int = 42;
        obj.b1_float = 3.14f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Base1>(json);

        REQUIRE(result->b1_int == 42);
        REQUIRE(result->b1_float == Catch::Approx(3.14f).epsilon(1e-4));
    }
}
