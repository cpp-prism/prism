#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("prismJson - shared_ptr<struct> field round trip", "[json][struct][shared-ptr]")
{
    SECTION("tst_struct my_shared_sub null round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        // my_shared_sub defaults to null

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_sub == nullptr);
    }

    SECTION("tst_struct my_shared_sub non-null round trip")
    {
        tst_struct obj;
        obj.my_int = 10;
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 42;
        obj.my_shared_sub->my_bool = true;
        obj.my_shared_sub->my_string = "sub_value";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 42);
        REQUIRE(result->my_shared_sub->my_bool == true);
        REQUIRE(result->my_shared_sub->my_string == "sub_value");
    }

    SECTION("tst_struct my_shared_self non-null round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 99;
        obj.my_shared_self->my_bool = true;
        obj.my_shared_self->my_string = "inner_str";
        // my_shared_self->my_shared_self is null by default, no infinite recursion

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 99);
        REQUIRE(result->my_shared_self->my_bool == true);
        REQUIRE(result->my_shared_self->my_string == "inner_str");
    }
}
