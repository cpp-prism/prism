#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries with bool and lang fields round trip", "[json][map][bool][enum][combo]")
{
    SECTION("my_map entry with bool true and lang round trip")
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
        child.my_bool = true;
        child.lang = language::english;
        obj.my_map.emplace("node1", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("node1").my_bool == true);
        REQUIRE(result->my_map.at("node1").lang == language::english);
    }

    SECTION("root bool and lang with map entry round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_bool = false;
        obj.lang = language::SimplifiedChinese;

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_bool = true;
        child.lang = language::unknow;
        obj.my_map.emplace("child", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_map.at("child").my_bool == true);
        REQUIRE(result->my_map.at("child").lang == language::unknow);
    }
}
