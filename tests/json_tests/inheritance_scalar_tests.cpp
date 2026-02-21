#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - Derived and Base classes scalar field combinations round trip", "[json][inheritance][scalar]")
{
    SECTION("Derived with all base and derived fields round trip")
    {
        Derived obj;
        obj.b1_int = 11;
        obj.b1_float = 1.1f;
        obj.b2_int = 22;
        obj.b2_float = 2.2f;
        obj.d_int = 33;
        obj.d_float = 3.3f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Derived>(json);

        REQUIRE(result->b1_int == 11);
        REQUIRE(result->b2_int == 22);
        REQUIRE(result->d_int == 33);
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
    }

    SECTION("Base1 standalone with negative float round trip")
    {
        Base1 obj;
        obj.b1_int = -5;
        obj.b1_float = -0.5f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Base1>(json);

        REQUIRE(result->b1_int == -5);
    }

    SECTION("Base2 standalone with large values round trip")
    {
        Base2 obj;
        obj.b2_int = 10000;
        obj.b2_float = 1000.5f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<Base2>(json);

        REQUIRE(result->b2_int == 10000);
    }
}
