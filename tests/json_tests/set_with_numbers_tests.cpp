#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_set_str with various string values round trip", "[json][set][string]")
{
    SECTION("my_set_str with number-like strings round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"123", "456", "789"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("123") == 1);
        REQUIRE(result->my_set_str.count("456") == 1);
        REQUIRE(result->my_set_str.count("789") == 1);
    }

    SECTION("my_set_str with boolean-like strings round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"true", "false", "yes", "no"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 4);
        REQUIRE(result->my_set_str.count("true") == 1);
        REQUIRE(result->my_set_str.count("false") == 1);
        REQUIRE(result->my_set_str.count("yes") == 1);
        REQUIRE(result->my_set_str.count("no") == 1);
    }

    SECTION("my_set_str with unicode strings round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"中文", "日本語", "한국어"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("中文") == 1);
        REQUIRE(result->my_set_str.count("日本語") == 1);
        REQUIRE(result->my_set_str.count("한국어") == 1);
    }

    SECTION("my_set_str with null-like strings round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"null", "~", "NULL"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("null") == 1);
        REQUIRE(result->my_set_str.count("~") == 1);
        REQUIRE(result->my_set_str.count("NULL") == 1);
    }
}
