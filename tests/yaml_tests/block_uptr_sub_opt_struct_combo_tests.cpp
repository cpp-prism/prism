#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_uptr_sub and my_opt_struct combined round trip", "[yaml][block][unique_ptr][optional][struct][combo]")
{
    SECTION("my_uptr_sub non-null and my_opt_struct with value block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 111;
        obj.my_uptr_sub->my_string = "block_uptr_val";

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 222;
        obj.my_opt_struct->my_string = "block_opt_val";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 111);
        REQUIRE(result->my_uptr_sub->my_string == "block_uptr_val");
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 222);
        REQUIRE(result->my_opt_struct->my_string == "block_opt_val");
    }

    SECTION("my_uptr_sub with vec_sp alongside my_opt_struct block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 333;

        tst_sub_struct child;
        child.my_int = 444;
        child.my_string = "block_child";
        obj.my_uptr_sub->my_vec_sp.push_back(child);

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 555;
        obj.my_opt_struct->my_opt_int = 99;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_uptr_sub->my_int == 333);
        REQUIRE(result->my_uptr_sub->my_vec_sp.size() == 1);
        REQUIRE(result->my_uptr_sub->my_vec_sp[0].my_int == 444);
        REQUIRE(result->my_opt_struct->my_int == 555);
        REQUIRE(*result->my_opt_struct->my_opt_int == 99);
    }
}
