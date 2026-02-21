#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - optional<struct> round trip", "[json][optional][struct]")
{
    SECTION("optional<tst_sub_struct> with value round trip")
    {
        tst_struct original;
        original.my_opt_struct = tst_sub_struct{};
        original.my_opt_struct->my_int = 42;
        original.my_opt_struct->my_bool = true;
        original.my_opt_struct->my_string = "opt_value";

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("opt_value") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 42);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "opt_value");
    }

    SECTION("optional<tst_sub_struct> null round trip")
    {
        tst_struct original;
        original.my_opt_struct = std::nullopt;

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_opt_struct\":null") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("standalone optional<tst_sub_struct> with value")
    {
        std::optional<tst_sub_struct> original;
        original = tst_sub_struct{};
        original->my_int = 99;
        original->my_string = "standalone";

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::optional<tst_sub_struct>>(json);
        REQUIRE(result->has_value());
        REQUIRE((*result)->my_int == 99);
        REQUIRE((*result)->my_string == "standalone");
    }

    SECTION("standalone optional<tst_sub_struct> null")
    {
        std::optional<tst_sub_struct> original = std::nullopt;
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json == "null");
        auto result = prism::json::fromJsonString<std::optional<tst_sub_struct>>(json);
        REQUIRE(!result->has_value());
    }
}
