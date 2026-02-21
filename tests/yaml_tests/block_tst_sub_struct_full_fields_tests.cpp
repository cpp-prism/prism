#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_sub_struct all fields round trip", "[yaml][block][struct][substruct]")
{
    SECTION("tst_sub_struct all scalar fields populated block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 42;
        obj.my_bool = true;
        obj.my_float = 1.5f;
        obj.my_double = 2.75;
        obj.my_string = "full_sub";
        obj.my_longlong = 9876543210LL;
        obj.my_opt_str = "opt_value";
        obj.my_opt_int = 7;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "full_sub");
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "opt_value");
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 7);
    }

    SECTION("tst_sub_struct with opt fields null block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 5;
        obj.my_bool = false;
        obj.my_string = "no_opts";
        // my_opt_str and my_opt_int remain nullopt

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "no_opts");
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(!result->my_opt_int.has_value());
    }

    SECTION("tst_sub_struct with my_vec_sp containing element with all fields block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_string = "parent_sub";

        tst_sub_struct child;
        child.my_int = 99;
        child.my_bool = true;
        child.my_string = "child_sub";
        child.my_longlong = 123456789LL;
        child.my_opt_str = "child_opt";
        obj.my_vec_sp.push_back(child);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_string == "parent_sub");
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "child_sub");
        REQUIRE(result->my_vec_sp[0].my_longlong == 123456789LL);
        REQUIRE(result->my_vec_sp[0].my_opt_str.has_value());
        REQUIRE(*result->my_vec_sp[0].my_opt_str == "child_opt");
    }
}
