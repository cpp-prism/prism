#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_opt_str with varied content round trip", "[json][opt_str][large]")
{
    SECTION("my_opt_str with long string round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "This is a longer optional string with multiple words.";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "This is a longer optional string with multiple words.");
    }

    SECTION("my_opt_str with special chars round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "key=value&other=123";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "key=value&other=123");
    }

    SECTION("my_opt_str nullopt alongside non-empty string fields round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_string = "primary_str";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = std::nullopt;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_string == "primary_str");
    }
}
