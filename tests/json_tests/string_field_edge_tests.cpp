#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_string edge cases round trip", "[json][string][edge]")
{
    SECTION("my_string with spaces only round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_string = "   ";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "   ");
    }

    SECTION("my_string with numbers round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_string = "123456";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "123456");
    }

    SECTION("my_string with underscore and dash round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_string = "hello_world-test";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "hello_world-test");
    }

    SECTION("tst_sub_struct my_string with mixed ASCII round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 4;
        obj.my_string = "A1_b2-C3";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_string == "A1_b2-C3");
    }
}
