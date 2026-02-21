#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - large list and vector stress tests", "[json][list][vector][stress]")
{
    SECTION("my_list_int with 100 elements round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 100; ++i)
            obj.my_list_int.push_back(i * 3);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 100);
        int i = 0;
        for (auto v : result->my_list_int)
        {
            REQUIRE(v == i * 3);
            ++i;
        }
    }

    SECTION("my_vec_sp with 50 elements round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 50; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_string = "item_" + std::to_string(i);
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 50);
        for (int i = 0; i < 50; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == i);
            REQUIRE(result->my_vec_sp[i].my_string == "item_" + std::to_string(i));
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }

    SECTION("my_deque_int with 200 elements round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 200; ++i)
            obj.my_deque_int.push_back(i * i);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 200);
        for (int i = 0; i < 200; ++i)
            REQUIRE(result->my_deque_int[i] == i * i);
    }
}
