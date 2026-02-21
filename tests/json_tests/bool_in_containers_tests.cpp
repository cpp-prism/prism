#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_bool in various struct contexts round trip", "[json][bool][struct]")
{
    SECTION("my_bool = true in root tst_struct round trip")
    {
        tst_struct obj;
        obj.my_bool = true;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
    }

    SECTION("my_bool = false in embedded my_struct round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_bool = false;
        obj.my_struct.my_int = 5;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_bool == false);
        REQUIRE(result->my_struct.my_int == 5);
    }

    SECTION("my_bool alternating in my_vec_sp elements round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 4; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 4);
        for (int i = 0; i < 4; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == i);
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }

    SECTION("my_bool in my_unordermap values round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_bool = true;
        obj.my_unordermap["t"] = s1;

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_bool = false;
        obj.my_unordermap["f"] = s2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap["t"].my_bool == true);
        REQUIRE(result->my_unordermap["f"].my_bool == false);
    }
}
