#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_struct my_struct.my_vec_sp multi-element round trip", "[json][struct][embedded][vector]")
{
    SECTION("my_struct.my_vec_sp with 3 elements round trip")
    {
        tst_struct obj;
        obj.my_int = 10;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 100;

        for (int i = 1; i <= 3; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 7;
            sub.my_bool = (i % 2 == 1);
            sub.my_string = "sub_" + std::to_string(i);
            obj.my_struct.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            REQUIRE(result->my_struct.my_vec_sp[i].my_int == (i + 1) * 7);
            REQUIRE(result->my_struct.my_vec_sp[i].my_bool == ((i + 1) % 2 == 1));
            REQUIRE(result->my_struct.my_vec_sp[i].my_string == "sub_" + std::to_string(i + 1));
        }
    }

    SECTION("my_struct.my_vec_sp nested: elements with their own my_vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 11;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 200;

        tst_sub_struct parent_sub;
        parent_sub.my_int = 50;
        parent_sub.my_string = "parent_sub";

        tst_sub_struct child_sub;
        child_sub.my_int = 51;
        child_sub.my_string = "child_of_parent";
        parent_sub.my_vec_sp.push_back(child_sub);

        obj.my_struct.my_vec_sp.push_back(parent_sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 200);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 50);
        REQUIRE(result->my_struct.my_vec_sp[0].my_string == "parent_sub");
        REQUIRE(result->my_struct.my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_vec_sp[0].my_int == 51);
        REQUIRE(result->my_struct.my_vec_sp[0].my_vec_sp[0].my_string == "child_of_parent");
    }
}
