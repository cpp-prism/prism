#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - unique_ptr<struct> field round trip", "[json][unique_ptr][struct]")
{
    SECTION("my_uptr_sub non-null round trip")
    {
        tst_struct original;
        original.my_uptr_sub = std::make_unique<tst_sub_struct>();
        original.my_uptr_sub->my_int = 77;
        original.my_uptr_sub->my_bool = true;
        original.my_uptr_sub->my_string = "uptr_value";

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("uptr_value") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 77);
        REQUIRE(result->my_uptr_sub->my_bool == true);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_value");
    }

    SECTION("my_uptr_sub null round trip")
    {
        tst_struct original;
        original.my_uptr_sub = nullptr;

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_uptr_sub\":null") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_uptr_sub == nullptr);
    }

    SECTION("standalone unique_ptr<tst_sub_struct> non-null")
    {
        auto original = std::make_unique<tst_sub_struct>();
        original->my_int = 55;
        original->my_string = "standalone_uptr";

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::unique_ptr<tst_sub_struct>>(json);
        REQUIRE(*result != nullptr);
        REQUIRE((*result)->my_int == 55);
        REQUIRE((*result)->my_string == "standalone_uptr");
    }

    SECTION("standalone unique_ptr<tst_sub_struct> null")
    {
        std::unique_ptr<tst_sub_struct> original = nullptr;
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json == "null");

        auto result = prism::json::fromJsonString<std::unique_ptr<tst_sub_struct>>(json);
        REQUIRE(*result == nullptr);
    }
}
