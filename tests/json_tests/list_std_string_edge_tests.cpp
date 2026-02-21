#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_std_string with various string values round trip", "[json][list][string][edge]")
{
    SECTION("my_list_std_string with various content types round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"hello", "world", "foo"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it == "hello"); ++it;
        REQUIRE(*it == "world"); ++it;
        REQUIRE(*it == "foo");
    }

    SECTION("my_list_std_string with empty element and unicode round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"", "中文", "한국어", "日本語"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 4);
        auto it = result->my_list_std_string.begin();
        REQUIRE(it->empty()); ++it;
        REQUIRE(*it == "中文"); ++it;
        REQUIRE(*it == "한국어"); ++it;
        REQUIRE(*it == "日本語");
    }

    SECTION("my_list_std_string alongside my_list_int round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int = {10, 20, 30, 40};
        obj.my_list_std_string = {"a", "b", "c"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 4);
        REQUIRE(result->my_list_std_string.size() == 3);
        auto sit = result->my_list_std_string.begin();
        REQUIRE(*sit == "a");
    }
}
