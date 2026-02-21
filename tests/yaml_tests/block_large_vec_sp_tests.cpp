#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp with many elements round trip", "[yaml][block][vec][large][stress]")
{
    SECTION("my_vec_sp with 8 elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 8; ++i) {
            tst_sub_struct s;
            s.my_int = i + 1;
            s.my_longlong = static_cast<long long>(i * 500);
            s.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(s);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 8);
        for (int i = 0; i < 8; ++i) {
            REQUIRE(result->my_vec_sp[i].my_int == i + 1);
            REQUIRE(result->my_vec_sp[i].my_longlong == static_cast<long long>(i * 500));
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }

    SECTION("my_vec_sp with string fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 4; ++i) {
            tst_sub_struct s;
            s.my_int = i * 5;
            s.my_string = "block_item_" + std::to_string(i);
            obj.my_vec_sp.push_back(s);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 4);
        for (int i = 0; i < 4; ++i) {
            REQUIRE(result->my_vec_sp[i].my_int == i * 5);
            REQUIRE(result->my_vec_sp[i].my_string == "block_item_" + std::to_string(i));
        }
    }
}
