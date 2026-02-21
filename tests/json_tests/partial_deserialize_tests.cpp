#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - partial deserialization", "[json][partial][struct]")
{
    SECTION("extra unknown fields are ignored")
    {
        // JSON with fields not in tst_sub_struct
        std::string json = R"({"my_int":5,"my_bool":true,"my_string":"hello","unknown_field":"ignored","extra_num":999})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "hello");
    }

    SECTION("missing fields use default values")
    {
        // JSON with only some fields present
        std::string json = R"({"my_int":42})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == false);   // default
        REQUIRE(result->my_string == "");    // default
    }

    SECTION("empty JSON object uses all defaults")
    {
        std::string json = R"({})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "");
    }
}
