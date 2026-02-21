#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries each containing other containers round trip", "[json][map][nested][combo]")
{
    SECTION("my_map child with deque_int and set_str round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 10;
        child.my_deque_int = {1, 2, 3};
        child.my_set_str = {"x", "y"};
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("nested", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("nested").my_deque_int.size() == 3);
        REQUIRE(result->my_map.at("nested").my_set_str.count("x") == 1);
    }

    SECTION("my_map child with vec_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 20;
        child.my_vec_enum = {language::english, language::SimplifiedChinese};
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("langs", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("langs").my_vec_enum.size() == 2);
        REQUIRE(result->my_map.at("langs").my_vec_enum[0] == language::english);
    }

    SECTION("my_map two children with different containers round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct c1;
        c1.my_int = 1;
        c1.my_deque_int = {10, 20};
        c1.my_list_int.clear();
        c1.my_list_std_string.clear();
        obj.my_map.emplace("c1", std::move(c1));

        tst_struct c2;
        c2.my_int = 2;
        c2.my_set_str = {"p", "q"};
        c2.my_list_int.clear();
        c2.my_list_std_string.clear();
        obj.my_map.emplace("c2", std::move(c2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("c1").my_deque_int.size() == 2);
        REQUIRE(result->my_map.at("c2").my_set_str.count("p") == 1);
    }
}
