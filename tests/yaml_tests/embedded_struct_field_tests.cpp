#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - embedded struct field round trip", "[yaml][struct][embedded]")
{
    SECTION("tst_struct my_struct field round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_struct.my_int = 111;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_float = 2.5f;
        obj.my_struct.my_string = "embedded_val";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_struct.my_int == 111);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(std::abs(result->my_struct.my_float - 2.5f) < 0.001f);
        REQUIRE(result->my_struct.my_string == "embedded_val");
    }

    SECTION("tst_sub_struct my_vec_sp recursive nested round trip")
    {
        tst_sub_struct outer;
        outer.my_int = 1;

        tst_sub_struct inner1;
        inner1.my_int = 10;
        inner1.my_bool = true;
        inner1.my_string = "inner_a";
        outer.my_vec_sp.push_back(inner1);

        tst_sub_struct inner2;
        inner2.my_int = 20;
        inner2.my_bool = false;
        inner2.my_string = "inner_b";
        outer.my_vec_sp.push_back(inner2);

        std::string yaml = prism::yaml::toYamlStringFlow(outer);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "inner_a");
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "inner_b");
    }
}
