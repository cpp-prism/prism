#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_unordermap with multiple sub_struct entries round trip", "[json][unordermap][large]")
{
    SECTION("my_unordermap four entries with different int values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        for (int i = 1; i <= 4; ++i) {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            obj.my_unordermap["key" + std::to_string(i)] = sub;
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 4);
        REQUIRE(result->my_unordermap.at("key1").my_int == 10);
        REQUIRE(result->my_unordermap.at("key4").my_int == 40);
    }

    SECTION("my_unordermap sub_struct with bool and string round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct sub1;
        sub1.my_int = 1;
        sub1.my_bool = true;
        sub1.my_string = "one";
        obj.my_unordermap["a"] = sub1;

        tst_sub_struct sub2;
        sub2.my_int = 2;
        sub2.my_bool = false;
        sub2.my_string = "two";
        obj.my_unordermap["b"] = sub2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap.at("a").my_bool == true);
        REQUIRE(result->my_unordermap.at("a").my_string == "one");
        REQUIRE(result->my_unordermap.at("b").my_bool == false);
        REQUIRE(result->my_unordermap.at("b").my_string == "two");
    }

    SECTION("my_unordermap empty round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.empty());
    }
}
