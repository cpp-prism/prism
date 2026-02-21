#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format recursive shared_ptr<tst_struct> chain round trip", "[yaml][block][shared_ptr][recursive]")
{
    SECTION("tst_struct my_shared_self 2-level chain block round trip")
    {
        tst_struct outer;
        outer.my_int = 1;
        outer.my_list_int.clear();
        outer.my_list_std_string.clear();

        outer.my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_int = 2;
        outer.my_shared_self->my_string = "level_2_block";
        outer.my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_list_std_string.clear();

        outer.my_shared_self->my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_shared_self->my_int = 3;
        outer.my_shared_self->my_shared_self->my_string = "level_3_block";
        outer.my_shared_self->my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_shared_self->my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(outer);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 2);
        REQUIRE(result->my_shared_self->my_string == "level_2_block");
        REQUIRE(result->my_shared_self->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_shared_self->my_int == 3);
        REQUIRE(result->my_shared_self->my_shared_self->my_string == "level_3_block");
        REQUIRE(result->my_shared_self->my_shared_self->my_shared_self == nullptr);
    }

    SECTION("tst_struct my_shared_self with my_vec_sp populated in child block round trip")
    {
        tst_struct outer;
        outer.my_int = 10;
        outer.my_list_int.clear();
        outer.my_list_std_string.clear();

        outer.my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_int = 20;
        outer.my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_list_std_string.clear();

        tst_sub_struct child_sub;
        child_sub.my_int = 99;
        child_sub.my_string = "child_block_str";
        outer.my_shared_self->my_vec_sp.push_back(child_sub);

        std::string yaml = prism::yaml::toYamlStringBlock(outer);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_vec_sp.size() == 1);
        REQUIRE(result->my_shared_self->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_shared_self->my_vec_sp[0].my_string == "child_block_str");
    }
}
