#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format vec_sp with many elements mixed fields round trip", "[yaml][block][vec_sp][large][sub_struct]")
{
    SECTION("vec_sp 6 elements alternating bool block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 6; ++i) {
            tst_sub_struct sub;
            sub.my_int = i + 1;
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 6);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[5].my_int == 6);
    }

    SECTION("vec_sp 4 elements with string and longlong block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 4; ++i) {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_string = "item" + std::to_string(i);
            sub.my_longlong = static_cast<long long>(i) * 500LL;
            obj.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 4);
        REQUIRE(result->my_vec_sp[0].my_string == "item0");
        REQUIRE(result->my_vec_sp[3].my_string == "item3");
        REQUIRE(result->my_vec_sp[2].my_longlong == 1000LL);
    }
}
