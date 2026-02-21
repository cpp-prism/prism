#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries with my_list_int populated round trip", "[json][map][list_int][combo]")
{
    SECTION("my_map entry with list_int populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 10;
        child.my_list_int = {100, 200, 300};
        child.my_list_std_string.clear();
        obj.my_map.emplace("entry", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("entry").my_int == 10);
        REQUIRE(result->my_map.at("entry").my_list_int.size() == 3);
        auto it = result->my_map.at("entry").my_list_int.begin();
        REQUIRE(*it == 100); ++it;
        REQUIRE(*it == 200); ++it;
        REQUIRE(*it == 300);
    }

    SECTION("my_map two entries each with list_int round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct c1;
        c1.my_int = 1;
        c1.my_list_int = {1, 2};
        c1.my_list_std_string.clear();
        obj.my_map.emplace("first", std::move(c1));

        tst_struct c2;
        c2.my_int = 2;
        c2.my_list_int = {3, 4, 5};
        c2.my_list_std_string.clear();
        obj.my_map.emplace("second", std::move(c2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("first").my_list_int.size() == 2);
        REQUIRE(result->my_map.at("second").my_list_int.size() == 3);
    }
}
