#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_uptr_sub with full sub-struct fields round trip", "[yaml][flow][unique_ptr][substruct]")
{
    SECTION("my_uptr_sub with all scalar fields flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 99;
        obj.my_uptr_sub->my_bool = true;
        obj.my_uptr_sub->my_float = 2.5f;
        obj.my_uptr_sub->my_double = 3.75;
        obj.my_uptr_sub->my_string = "uptr_flow_full";
        obj.my_uptr_sub->my_longlong = 777777LL;
        obj.my_uptr_sub->my_opt_str = "uptr_opt_val";
        obj.my_uptr_sub->my_opt_int = 42;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 99);
        REQUIRE(result->my_uptr_sub->my_bool == true);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_flow_full");
        REQUIRE(result->my_uptr_sub->my_longlong == 777777LL);
        REQUIRE(result->my_uptr_sub->my_opt_str.has_value());
        REQUIRE(*result->my_uptr_sub->my_opt_str == "uptr_opt_val");
        REQUIRE(result->my_uptr_sub->my_opt_int.has_value());
        REQUIRE(*result->my_uptr_sub->my_opt_int == 42);
    }

    SECTION("my_uptr_sub with my_vec_sp populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 10;
        obj.my_uptr_sub->my_string = "uptr_with_vec";

        tst_sub_struct child;
        child.my_int = 11;
        child.my_string = "vec_child_of_uptr";
        child.my_bool = true;
        obj.my_uptr_sub->my_vec_sp.push_back(child);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 10);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_with_vec");
        REQUIRE(result->my_uptr_sub->my_vec_sp.size() == 1);
        REQUIRE(result->my_uptr_sub->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_uptr_sub->my_vec_sp[0].my_string == "vec_child_of_uptr");
        REQUIRE(result->my_uptr_sub->my_vec_sp[0].my_bool == true);
    }
}
