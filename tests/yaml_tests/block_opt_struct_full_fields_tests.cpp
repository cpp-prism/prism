#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_opt_struct (optional<tst_sub_struct>) full fields round trip", "[yaml][block][optional][struct]")
{
    SECTION("my_opt_struct with all scalar fields populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 42;
        sub.my_bool = true;
        sub.my_string = "block_opt_full";
        sub.my_longlong = 99999LL;
        sub.my_opt_str = "block_inner_opt";
        sub.my_opt_int = 7;
        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 42);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "block_opt_full");
        REQUIRE(result->my_opt_struct->my_longlong == 99999LL);
        REQUIRE(result->my_opt_struct->my_opt_str.has_value());
        REQUIRE(*result->my_opt_struct->my_opt_str == "block_inner_opt");
        REQUIRE(result->my_opt_struct->my_opt_int.has_value());
        REQUIRE(*result->my_opt_struct->my_opt_int == 7);
    }

    SECTION("my_opt_struct with nested my_vec_sp block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "block_opt_with_vec";

        tst_sub_struct child;
        child.my_int = 77;
        child.my_string = "block_vec_child";
        child.my_bool = true;
        sub.my_vec_sp.push_back(child);

        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 10);
        REQUIRE(result->my_opt_struct->my_string == "block_opt_with_vec");
        REQUIRE(result->my_opt_struct->my_vec_sp.size() == 1);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_int == 77);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_string == "block_vec_child");
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_bool == true);
    }

    SECTION("my_opt_struct nullopt block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(!result->my_opt_struct.has_value());
    }
}
