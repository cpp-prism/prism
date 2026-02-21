#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - full tst_struct block format round trip", "[yaml][block][full][struct]")
{
    SECTION("tst_struct with vec_sp and list_int block round trip")
    {
        tst_struct obj;
        obj.my_int = 42;
        obj.my_bool = true;
        obj.my_string = "hello_block";

        // Vector of sub-structs
        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_bool = true;
        sub1.my_string = "sub_one";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_bool = false;
        sub2.my_string = "sub_two";
        obj.my_vec_sp.push_back(sub2);

        // Clear default list values to simplify test
        obj.my_list_int = {1, 2, 3};
        obj.my_list_std_string = {"x", "y"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "hello_block");
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_string == "sub_one");
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_vec_sp[1].my_string == "sub_two");
        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_list_std_string.size() == 2);
    }

    SECTION("tst_struct all containers empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 7;
        obj.my_bool = false;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_list_std_string.empty());
        REQUIRE(result->my_deque_int.empty());
        REQUIRE(result->my_set_str.empty());
    }

    SECTION("tst_sub_struct with nested my_vec_sp block round trip")
    {
        tst_sub_struct outer;
        outer.my_int = 1;

        tst_sub_struct inner1;
        inner1.my_int = 11;
        inner1.my_bool = true;
        inner1.my_string = "nested_a";
        outer.my_vec_sp.push_back(inner1);

        tst_sub_struct inner2;
        inner2.my_int = 22;
        inner2.my_bool = false;
        inner2.my_string = "nested_b";
        outer.my_vec_sp.push_back(inner2);

        std::string yaml = prism::yaml::toYamlStringBlock(outer);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "nested_a");
        REQUIRE(result->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "nested_b");
    }
}
