#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_opt_str with varied content round trip", "[yaml][block][opt_str][large]")
{
    SECTION("my_opt_str with long string block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "This is a longer optional string for YAML.";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "This is a longer optional string for YAML.");
    }

    SECTION("my_opt_str with alphanumeric content block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "abc123XYZ";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "abc123XYZ");
    }

    SECTION("my_opt_str nullopt alongside non-empty string block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_string = "primary_yaml_str";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_string == "primary_yaml_str");
    }
}
