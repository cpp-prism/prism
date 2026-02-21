#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_unordermap with various tst_sub_struct values round trip", "[json][unordermap][string][combo]")
{
    SECTION("my_unordermap single entry round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_longlong = 100LL;
        obj.my_unordermap["entry1"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap.at("entry1").my_int == 10);
        REQUIRE(result->my_unordermap.at("entry1").my_longlong == 100LL);
    }

    SECTION("my_unordermap multiple entries round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct s1;
        s1.my_int = 100;
        s1.my_longlong = 1000LL;
        obj.my_unordermap["alpha"] = s1;

        tst_sub_struct s2;
        s2.my_int = 200;
        s2.my_longlong = 2000LL;
        obj.my_unordermap["beta"] = s2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap.at("alpha").my_int == 100);
        REQUIRE(result->my_unordermap.at("alpha").my_longlong == 1000LL);
        REQUIRE(result->my_unordermap.at("beta").my_int == 200);
        REQUIRE(result->my_unordermap.at("beta").my_longlong == 2000LL);
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
