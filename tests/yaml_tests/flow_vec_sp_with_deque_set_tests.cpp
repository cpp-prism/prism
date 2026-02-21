#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_vec_sp combined with my_deque_int and my_set_str round trip", "[yaml][flow][vec][deque][set][struct]")
{
    SECTION("my_vec_sp 2 elements with my_deque_int and my_set_str flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_string = "flow_sub1";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        obj.my_vec_sp.push_back(sub2);

        obj.my_deque_int = {4, 5, 6};
        obj.my_set_str = {"delta", "epsilon"};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[2] == 6);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("delta") == 1);
    }
}
