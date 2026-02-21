#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format tst_sub_struct float and string field combinations round trip", "[yaml][block][sub_struct][float][string]")
{
    SECTION("tst_sub_struct float positive and non-empty string block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_float = 2.5f;
        obj.my_string = "hello_yaml_sub";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_float == Catch::Approx(2.5f));
        REQUIRE(result->my_string == "hello_yaml_sub");
    }

    SECTION("tst_sub_struct float negative and empty string block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_float = -0.75f;
        obj.my_string = "";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_float == Catch::Approx(-0.75f));
        REQUIRE(result->my_string == "");
    }

    SECTION("tst_sub_struct in vec_sp with float and string block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_float = 1.5f;
        sub.my_string = "in_yaml_vec";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp[0].my_float == Catch::Approx(1.5f));
        REQUIRE(result->my_vec_sp[0].my_string == "in_yaml_vec");
    }
}
