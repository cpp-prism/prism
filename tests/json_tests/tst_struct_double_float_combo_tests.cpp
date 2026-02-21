#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_float and my_double various values together round trip", "[json][float][double][combo]")
{
    SECTION("my_float and my_double both positive non-trivial round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_float = 3.14f;
        obj.my_double = 2.71828;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(3.14f).epsilon(0.001));
        REQUIRE(result->my_double == Catch::Approx(2.71828).epsilon(0.0001));
    }

    SECTION("my_float negative and my_double zero round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_float = -1.5f;
        obj.my_double = 0.0;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(-1.5f));
        REQUIRE(result->my_double == 0.0);
    }

    SECTION("my_float zero and my_double negative round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_float = 0.0f;
        obj.my_double = -99.99;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_float == 0.0f);
        REQUIRE(result->my_double == Catch::Approx(-99.99).epsilon(0.001));
    }
}
