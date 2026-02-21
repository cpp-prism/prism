#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format optional<tst_sub_struct> (my_opt_struct) round trip", "[yaml][flow][optional][struct]")
{
    SECTION("my_opt_struct with full fields flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 42;
        sub.my_bool = true;
        sub.my_string = "opt_flow_full";
        sub.my_longlong = 12345LL;
        sub.my_opt_str = "inner_opt";
        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 42);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "opt_flow_full");
        REQUIRE(result->my_opt_struct->my_longlong == 12345LL);
        REQUIRE(result->my_opt_struct->my_opt_str.has_value());
        REQUIRE(*result->my_opt_struct->my_opt_str == "inner_opt");
    }

    SECTION("my_opt_struct nullopt flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_opt_struct is nullopt by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("my_opt_struct with my_vec_sp populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "opt_parent";

        tst_sub_struct child;
        child.my_int = 11;
        child.my_string = "opt_child";
        sub.my_vec_sp.push_back(child);

        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 10);
        REQUIRE(result->my_opt_struct->my_string == "opt_parent");
        REQUIRE(result->my_opt_struct->my_vec_sp.size() == 1);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_string == "opt_child");
    }
}
