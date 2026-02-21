#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <list>

TEST_CASE("prismJson - list<struct> round trip", "[json][list][struct]")
{
    SECTION("list<tst_sub_struct> serialization")
    {
        std::list<tst_sub_struct> lst;
        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_string = "first";
        lst.push_back(s1);
        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_string = "second";
        lst.push_back(s2);

        std::string json = prism::json::toJsonString(lst);
        REQUIRE(json.find("10") != std::string::npos);
        REQUIRE(json.find("first") != std::string::npos);
        REQUIRE(json.find("20") != std::string::npos);
        REQUIRE(json.find("second") != std::string::npos);
    }

    SECTION("list<tst_sub_struct> round trip")
    {
        std::list<tst_sub_struct> original;
        tst_sub_struct s1;
        s1.my_int = 11;
        s1.my_bool = true;
        s1.my_float = 1.5f;
        s1.my_string = "item_one";
        original.push_back(s1);
        tst_sub_struct s2;
        s2.my_int = 22;
        s2.my_bool = false;
        s2.my_float = 2.5f;
        s2.my_string = "item_two";
        original.push_back(s2);

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::list<tst_sub_struct>>(json);

        REQUIRE(result->size() == 2);
        auto it = result->begin();
        REQUIRE(it->my_int == 11);
        REQUIRE(it->my_bool == true);
        REQUIRE(it->my_string == "item_one");
        ++it;
        REQUIRE(it->my_int == 22);
        REQUIRE(it->my_bool == false);
        REQUIRE(it->my_string == "item_two");
    }

    SECTION("tst_struct my_list_sp field round trip")
    {
        tst_struct outer;
        outer.my_int = 5;

        tst_struct inner1;
        inner1.my_int = 100;
        inner1.my_bool = true;
        outer.my_list_sp.push_back(std::move(inner1));

        tst_struct inner2;
        inner2.my_int = 200;
        inner2.my_bool = false;
        outer.my_list_sp.push_back(std::move(inner2));

        std::string json = prism::json::toJsonString(outer);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 100);
        REQUIRE(it->my_bool == true);
        ++it;
        REQUIRE(it->my_int == 200);
        REQUIRE(it->my_bool == false);
    }
}
