#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - JSON attributes", "[json][attributes]")
{
    SECTION("test json alias")
    {
        tst_struct val;
        val.my_int = 111;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"MY_INT1\"") != std::string::npos);
    }

    SECTION("round trip with json alias")
    {
        tst_struct val;
        val.my_int = 111;
        std::string json = prism::json::toJsonString(val);
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_int == 111);
    }

    SECTION("test json ignore")
    {
        tst_sub_struct val;
        val.my_int = 1;
        val.my_string = "test";
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("my_int") != std::string::npos);
    }

    SECTION("deserialize from alias key directly")
    {
        std::string json = R"({"MY_INT1": 42, "my_bool": true})";
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
    }
}

TEST_CASE("prismJson - null handling", "[json][null]")
{
    SECTION("deserialize null int")
    {
        std::string json = "null";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 0);
    }

    SECTION("deserialize null double")
    {
        std::string json = "null";
        auto result = prism::json::fromJsonString<double>(json);
        REQUIRE(*result == 0.0);
    }

    SECTION("serialize null optional")
    {
        std::optional<int> val;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("null") != std::string::npos);
    }
}
