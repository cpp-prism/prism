#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_struct lang field with map containers round trip", "[json][lang][map][combo]")
{
    SECTION("lang english with non-empty map round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.lang = language::english;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("en_child", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("en_child").my_int == 5);
    }

    SECTION("lang SimplifiedChinese with map_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.lang = language::SimplifiedChinese;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["k1"] = language::english;
        obj.my_map_enum["k2"] = language::unknow;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("k1") == language::english);
    }

    SECTION("lang unknow with empty map round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.lang = language::unknow;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->lang == language::unknow);
        REQUIRE(result->my_map.empty());
    }
}
