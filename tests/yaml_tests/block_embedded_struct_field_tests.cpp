#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format embedded struct field round trip", "[yaml][block][struct][embedded]")
{
    SECTION("tst_struct my_struct field block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_struct.my_int = 111;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_float = 2.5f;
        obj.my_struct.my_string = "embedded_block";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_struct.my_int == 111);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(std::abs(result->my_struct.my_float - 2.5f) < 0.001f);
        REQUIRE(result->my_struct.my_string == "embedded_block");
    }

    SECTION("tst_struct my_struct field all-default block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        // my_struct uses all defaults

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_struct.my_int == 0);
        REQUIRE(result->my_struct.my_bool == false);
        REQUIRE(result->my_struct.my_string == "");
    }
}
