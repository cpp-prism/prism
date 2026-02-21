#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map with nested tst_struct values round trip", "[json][map][nested][struct]")
{
    SECTION("my_map entry with vec_sp populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 42;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 7;
        sub.my_longlong = 100LL;
        child.my_vec_sp.push_back(sub);
        obj.my_map.emplace("child1", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("child1").my_int == 42);
        REQUIRE(result->my_map.at("child1").my_vec_sp.size() == 1);
        REQUIRE(result->my_map.at("child1").my_vec_sp[0].my_int == 7);
        REQUIRE(result->my_map.at("child1").my_vec_sp[0].my_longlong == 100LL);
    }

    SECTION("my_map entry with my_deque_int and my_set_str round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_deque_int = {10, 20, 30};
        child.my_set_str = {"x", "y"};
        obj.my_map.emplace("data", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("data").my_int == 99);
        REQUIRE(result->my_map.at("data").my_deque_int.size() == 3);
        REQUIRE(result->my_map.at("data").my_set_str.size() == 2);
    }

    SECTION("my_map two entries with different bool values round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct t;
        t.my_int = 1;
        t.my_list_int.clear();
        t.my_list_std_string.clear();
        t.my_bool = true;
        obj.my_map.emplace("true_entry", std::move(t));

        tst_struct f;
        f.my_int = 2;
        f.my_list_int.clear();
        f.my_list_std_string.clear();
        f.my_bool = false;
        obj.my_map.emplace("false_entry", std::move(f));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("true_entry").my_bool == true);
        REQUIRE(result->my_map.at("false_entry").my_bool == false);
    }
}
