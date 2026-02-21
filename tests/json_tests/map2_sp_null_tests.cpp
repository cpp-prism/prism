#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map2 (map<string,shared_ptr<tst_sub_struct>>) null and non-null entries round trip", "[json][map2][sptr][null]")
{
    SECTION("my_map2 single non-null entry round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 55;
        sub->my_bool = true;
        obj.my_map2["alive"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.at("alive") != nullptr);
        REQUIRE(result->my_map2.at("alive")->my_int == 55);
        REQUIRE(result->my_map2.at("alive")->my_bool == true);
    }

    SECTION("my_map2 null entry round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();
        obj.my_map2["dead"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.count("dead") == 1);
        REQUIRE(result->my_map2.at("dead") == nullptr);
    }

    SECTION("my_map2 two non-null entries with different fields round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto s1 = std::make_shared<tst_sub_struct>();
        s1->my_int = 10;
        s1->my_string = "s1str";
        obj.my_map2["first"] = s1;

        auto s2 = std::make_shared<tst_sub_struct>();
        s2->my_int = 20;
        s2->my_longlong = 99999LL;
        obj.my_map2["second"] = s2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.at("first")->my_string == "s1str");
        REQUIRE(result->my_map2.at("second")->my_longlong == 99999LL);
    }
}
