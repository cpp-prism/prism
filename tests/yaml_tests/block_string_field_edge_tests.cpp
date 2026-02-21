#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_string edge cases round trip", "[yaml][block][string][edge]")
{
    SECTION("my_string with numbers block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_string = "123456";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "123456");
    }

    SECTION("my_string with underscore and dash block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_string = "hello_world-test";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "hello_world-test");
    }

    SECTION("tst_sub_struct my_string with mixed ASCII block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_string = "A1_b2_C3";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_string == "A1_b2_C3");
    }
}
