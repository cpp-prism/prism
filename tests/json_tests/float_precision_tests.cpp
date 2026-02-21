#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - float and double precision round trip after formatfloat fix", "[json][float][double][precision]")
{
    SECTION("small decimal double values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_double = 0.001;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_double == Catch::Approx(0.001).epsilon(1e-9));
    }

    SECTION("very small double 0.0001 round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_double = 0.0001;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_double == Catch::Approx(0.0001).epsilon(1e-9));
    }

    SECTION("double 0.5 (exactly representable) round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_double = 0.5;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_double == Catch::Approx(0.5).epsilon(1e-9));
    }

    SECTION("float small value 0.001f round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_float = 0.001f;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(0.001f).epsilon(1e-4));
    }

    SECTION("double 0.123456789 round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_double = 0.123456789;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_double == Catch::Approx(0.123456789).epsilon(1e-9));
    }
}
