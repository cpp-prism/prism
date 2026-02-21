#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - bool deserialization case-insensitive", "[json][bool][case]")
{
    SECTION("deserialize 'True' as true from hand-crafted JSON")
    {
        std::string json = R"({"my_bool": True})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == true);
    }

    SECTION("deserialize 'TRUE' as true from hand-crafted JSON")
    {
        std::string json = R"({"my_bool": TRUE})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == true);
    }

    SECTION("deserialize 'false' as false from hand-crafted JSON")
    {
        std::string json = R"({"my_bool": false})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == false);
    }

    SECTION("deserialize 'False' as false from hand-crafted JSON")
    {
        std::string json = R"({"my_bool": False})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == false);
    }

    SECTION("tst_sub_struct round trip: my_bool=true serializes as 'true' and deserializes back")
    {
        tst_sub_struct obj;
        obj.my_bool = true;
        obj.my_int = 42;

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("true") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_int == 42);
    }

    SECTION("tst_sub_struct round trip: my_bool=false serializes as 'false' and deserializes back")
    {
        tst_sub_struct obj;
        obj.my_bool = false;
        obj.my_int = 7;

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("false") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_int == 7);
    }
}
