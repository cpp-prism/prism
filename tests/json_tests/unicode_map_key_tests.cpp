#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - Unicode (UTF-8) characters as map keys round trip", "[json][map][unicode][utf8]")
{
    SECTION("map<string,tst_sub_struct> with Chinese map keys JSON round trip")
    {
        std::map<std::string, tst_sub_struct> obj;

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "val_one";
        obj["中文键一"] = s1;

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "val_two";
        obj["中文键二"] = s2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<std::map<std::string, tst_sub_struct>>(json);

        REQUIRE(result->size() == 2);
        REQUIRE(result->count("中文键一") == 1);
        REQUIRE((*result)["中文键一"].my_int == 1);
        REQUIRE((*result)["中文键一"].my_string == "val_one");
        REQUIRE(result->count("中文键二") == 1);
        REQUIRE((*result)["中文键二"].my_int == 2);
        REQUIRE((*result)["中文键二"].my_string == "val_two");
    }

    SECTION("unordered_map<string, int> with Japanese map keys JSON round trip")
    {
        std::unordered_map<std::string, int> obj;
        obj["テスト一"] = 10;
        obj["テスト二"] = 20;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<std::unordered_map<std::string, int>>(json);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["テスト一"] == 10);
        REQUIRE((*result)["テスト二"] == 20);
    }
}
