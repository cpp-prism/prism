#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries with my_string field variations round trip", "[json][map][string][field]")
{
    SECTION("my_map entry with non-empty string in child round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_string = "child_string_value";
        obj.my_map.emplace("node", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("node").my_string == "child_string_value");
    }

    SECTION("root my_string alongside map round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_string = "root_string";

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_string = "nested_string";
        obj.my_map.emplace("item", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "root_string");
        REQUIRE(result->my_map.at("item").my_string == "nested_string");
    }
}
