#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_int and my_set_str together round trip", "[json][list_int][set_str][combo]")
{
    SECTION("list_int and set_str both non-empty round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_int = {100, 200, 300};
        obj.my_set_str = {"a", "b", "c"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("b") == 1);
    }

    SECTION("list_int with negatives and set_str with long strings round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_int = {-1, -2, -3};
        obj.my_set_str = {"longstring1", "longstring2"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_list_int.front() == -1);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("longstring1") == 1);
    }

    SECTION("list_int and set_str both empty round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_set_str.empty());
    }
}
