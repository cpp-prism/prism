#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

TEST_CASE("prismJson - float and double edge case round trips", "[json][float][double]")
{
    SECTION("my_float positive large value round trip")
    {
        tst_struct obj;
        obj.my_float = 1.0e10f;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(std::fabs(result->my_float - 1.0e10f) / 1.0e10f < 1e-5f);
    }

    SECTION("my_float negative value round trip")
    {
        tst_struct obj;
        obj.my_float = -3.14f;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(std::fabs(result->my_float - (-3.14f)) < 1e-4f);
    }

    SECTION("my_double precise value round trip")
    {
        tst_struct obj;
        obj.my_double = 1.23456789012345;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(std::fabs(result->my_double - 1.23456789012345) < 1e-9);
    }

    SECTION("my_double negative large value round trip")
    {
        tst_struct obj;
        obj.my_double = -9.87654321e15;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(std::fabs(result->my_double - (-9.87654321e15)) / 9.87654321e15 < 1e-6);
    }

    SECTION("my_float and my_double both zero round trip")
    {
        tst_struct obj;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_float == 0.0f);
        REQUIRE(result->my_double == 0.0);
    }
}
