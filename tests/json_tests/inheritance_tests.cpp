#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - inheritance", "[json][inheritance]")
{
    SECTION("serialize Derived class")
    {
        Derived d;
        d.b1_int = 10;
        d.b1_float = 1.5f;
        d.b2_int = 20;
        d.b2_float = 2.5f;
        d.d_int = 30;
        d.d_float = 3.5f;

        std::string json = prism::json::toJsonString(d);

        // 验证所有基类和派生类字段均出现在 JSON 中
        REQUIRE(json.find("b1_int") != std::string::npos);
        REQUIRE(json.find("b1_float") != std::string::npos);
        REQUIRE(json.find("b2_int") != std::string::npos);
        REQUIRE(json.find("b2_float") != std::string::npos);
        REQUIRE(json.find("d_int") != std::string::npos);
        REQUIRE(json.find("d_float") != std::string::npos);
        REQUIRE(json.find("10") != std::string::npos);
        REQUIRE(json.find("20") != std::string::npos);
        REQUIRE(json.find("30") != std::string::npos);
    }

    SECTION("deserialize Derived class")
    {
        std::string json = R"({"b1_int":11,"b1_float":1.1,"b2_int":22,"b2_float":2.2,"d_int":33,"d_float":3.3})";
        auto result = prism::json::fromJsonString<Derived>(json);
        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == 11);
        REQUIRE(result->b2_int == 22);
        REQUIRE(result->d_int == 33);
    }

    SECTION("round trip Derived class")
    {
        Derived original;
        original.b1_int = 100;
        original.b1_float = 1.25f;
        original.b2_int = 200;
        original.b2_float = 2.25f;
        original.d_int = 300;
        original.d_float = 3.25f;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<Derived>(json);
        REQUIRE(result != nullptr);
        REQUIRE(result->b1_int == original.b1_int);
        REQUIRE(result->b2_int == original.b2_int);
        REQUIRE(result->d_int == original.d_int);
        REQUIRE(result->b1_float == original.b1_float);
        REQUIRE(result->b2_float == original.b2_float);
        REQUIRE(result->d_float == original.d_float);
    }
}
