#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format optional<string> and optional<int> in struct", "[yaml][block][optional][string]")
{
    SECTION("tst_sub_struct my_opt_str with value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_opt_str = "hello_block";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "hello_block");
    }

    SECTION("tst_sub_struct my_opt_str nullopt block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 4;
        obj.my_opt_str = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(!result->my_opt_str.has_value());
    }

    SECTION("tst_sub_struct my_opt_int = 0 block round trip (zero must not be read as null)")
    {
        tst_sub_struct obj;
        obj.my_int = 5;
        obj.my_opt_int = 0;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 0);
    }

    SECTION("tst_sub_struct my_opt_str empty string block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 6;
        obj.my_opt_str = "";  // some value, but empty string

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 6);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "");
    }
}
