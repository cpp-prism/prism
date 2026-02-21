#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_unordermap with my_vec_enum and my_map_enum together round trip", "[json][unordermap][vec_enum][combo]")
{
    SECTION("my_unordermap and my_vec_enum both populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};

        tst_sub_struct sub;
        sub.my_int = 5;
        obj.my_unordermap["key1"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap.at("key1").my_int == 5);
    }

    SECTION("my_unordermap and my_map_enum both populated round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();
        obj.my_map_enum["e1"] = language::english;
        obj.my_map_enum["e2"] = language::unknow;

        tst_sub_struct sub1;
        sub1.my_int = 10;
        obj.my_unordermap["u1"] = sub1;

        tst_sub_struct sub2;
        sub2.my_int = 20;
        obj.my_unordermap["u2"] = sub2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("e1") == language::english);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap.at("u1").my_int == 10);
        REQUIRE(result->my_unordermap.at("u2").my_int == 20);
    }
}
