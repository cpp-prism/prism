#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_struct (embedded tst_sub_struct) with various fields round trip", "[yaml][block][my_struct][combo]")
{
    SECTION("my_struct with bool and string block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 10;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "yaml_inner_str";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 10);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "yaml_inner_str");
    }

    SECTION("my_struct with opt_int and longlong block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 20;
        obj.my_struct.my_opt_int = 88;
        obj.my_struct.my_longlong = 99999LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 20);
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 88);
        REQUIRE(result->my_struct.my_longlong == 99999LL);
    }
}
