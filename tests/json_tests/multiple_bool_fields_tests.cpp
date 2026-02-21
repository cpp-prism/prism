#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_bool in nested structures across multiple levels round trip", "[json][bool][nested][struct]")
{
    SECTION("my_bool in root and vec_sp elements simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;

        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_bool = false;
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_bool = true;
        obj.my_vec_sp.push_back(sub2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_bool == true);
    }

    SECTION("my_bool in root, embedded my_struct, and shared_sub round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = false;
        obj.my_struct.my_bool = true;

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_bool = false;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_bool == false);
    }

    SECTION("my_bool in map values round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;

        tst_struct mv1;
        mv1.my_int = 10;
        mv1.my_list_int.clear();
        mv1.my_list_std_string.clear();
        mv1.my_bool = true;
        obj.my_map["true_entry"] = std::move(mv1);

        tst_struct mv2;
        mv2.my_int = 20;
        mv2.my_list_int.clear();
        mv2.my_list_std_string.clear();
        mv2.my_bool = false;
        obj.my_map["false_entry"] = std::move(mv2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_map["true_entry"].my_bool == true);
        REQUIRE(result->my_map["false_entry"].my_bool == false);
    }
}
