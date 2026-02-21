#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("prismJson - tst_struct my_vec_sp (vector<tst_sub_struct>) round trip", "[json][struct][vector]")
{
    SECTION("tst_struct my_vec_sp with two elements round trip")
    {
        tst_struct obj;
        obj.my_int = 7;

        tst_sub_struct e1;
        e1.my_int = 11;
        e1.my_bool = true;
        e1.my_string = "elem_one";
        obj.my_vec_sp.push_back(e1);

        tst_sub_struct e2;
        e2.my_int = 22;
        e2.my_bool = false;
        e2.my_string = "elem_two";
        obj.my_vec_sp.push_back(e2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "elem_one");
        REQUIRE(result->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "elem_two");
    }

    SECTION("standalone vector<tst_sub_struct> round trip")
    {
        std::vector<tst_sub_struct> original;
        tst_sub_struct s1;
        s1.my_int = 100;
        s1.my_bool = true;
        s1.my_float = 1.5f;
        s1.my_string = "first";
        original.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 200;
        s2.my_bool = false;
        s2.my_float = 2.5f;
        s2.my_string = "second";
        original.push_back(s2);

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::vector<tst_sub_struct>>(json);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].my_int == 100);
        REQUIRE((*result)[0].my_bool == true);
        REQUIRE((*result)[0].my_string == "first");
        REQUIRE((*result)[1].my_int == 200);
        REQUIRE((*result)[1].my_bool == false);
        REQUIRE((*result)[1].my_string == "second");
    }
}
