#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp combined with my_deque_int and my_set_str round trip", "[yaml][block][vec][deque][set][struct]")
{
    SECTION("my_vec_sp 2 elements with my_deque_int and my_set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_string = "block_sub1_deque_set";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_bool = true;
        obj.my_vec_sp.push_back(sub2);

        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"foo", "bar"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[2] == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("foo") == 1);
    }

    SECTION("vec_sp with nested opt fields alongside deque block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 50;
        sub.my_opt_int = 77;
        sub.my_opt_str = "block_sub_opt";
        obj.my_vec_sp.push_back(sub);

        obj.my_deque_int = {-5, 0, 5};
        obj.my_set_str = {"x", "y", "z"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(*result->my_vec_sp[0].my_opt_int == 77);
        REQUIRE(*result->my_vec_sp[0].my_opt_str == "block_sub_opt");
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.count("z") == 1);
    }
}
