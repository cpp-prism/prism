#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_opt_struct and my_uptr_sub together round trip", "[yaml][block][optional][unique_ptr][combo]")
{
    SECTION("both my_opt_struct and my_uptr_sub populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct opt_sub;
        opt_sub.my_int = 10;
        opt_sub.my_longlong = 100LL;
        obj.my_opt_struct = opt_sub;

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 20;
        obj.my_uptr_sub->my_longlong = 200LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 10);
        REQUIRE(result->my_opt_struct->my_longlong == 100LL);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 20);
        REQUIRE(result->my_uptr_sub->my_longlong == 200LL);
    }

    SECTION("my_opt_struct populated, my_uptr_sub null block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 77;
        obj.my_opt_struct = sub;
        obj.my_uptr_sub.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 77);
        REQUIRE(result->my_uptr_sub == nullptr);
    }
}
