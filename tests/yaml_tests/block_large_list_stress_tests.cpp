#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format large list and vector stress tests", "[yaml][block][list][vector][stress]")
{
    SECTION("my_list_int with 50 elements block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 50; ++i)
            obj.my_list_int.push_back(i * 5);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 50);
        int i = 0;
        for (auto v : result->my_list_int)
        {
            REQUIRE(v == i * 5);
            ++i;
        }
    }

    SECTION("my_vec_sp with 20 elements block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 20; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_string = "block_item_" + std::to_string(i);
            sub.my_bool = (i % 3 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 20);
        for (int i = 0; i < 20; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == i * 10);
            REQUIRE(result->my_vec_sp[i].my_string == "block_item_" + std::to_string(i));
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 3 == 0));
        }
    }

    SECTION("my_deque_int with 30 elements block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 30; ++i)
            obj.my_deque_int.push_back(i * i + 1);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 30);
        for (int i = 0; i < 30; ++i)
            REQUIRE(result->my_deque_int[i] == i * i + 1);
    }
}
