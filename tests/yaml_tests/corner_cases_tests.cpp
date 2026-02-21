#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - corner cases", "[yaml][corner]")
{
    SECTION("empty value in middle of yaml")
    {
        std::string yaml = "my_int: 123\nmy_vec_sp: \nmy_bool: true";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 123);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("empty value at beginning of yaml")
    {
        std::string yaml = "my_vec_sp: \nmy_int: 123\nmy_bool: true";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 123);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("empty value at end of yaml")
    {
        std::string yaml = "my_int: 123\nmy_bool: true\nmy_vec_sp: ";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 123);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("deserialize from block format with trailing empty line")
    {
        std::string yaml = "my_int: 456\nmy_bool: false\nmy_string: \"deserialize\"\n";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "deserialize");
    }

    SECTION("empty vector serialized as block")
    {
        std::vector<int> val;
        std::string yaml = prism::yaml::toYamlStringBlock(val, 2);
        bool isEmpty = yaml.find("-") == std::string::npos;
        REQUIRE(isEmpty);
    }

    SECTION("empty map serialized")
    {
        std::map<std::string, int> val;
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<std::map<std::string, int>>(yaml);
        REQUIRE(result->empty());
    }

    SECTION("optional empty string")
    {
        std::optional<std::string> val = std::optional<std::string>("");
        std::string yaml = prism::yaml::toYamlString(val);
        auto result = prism::yaml::fromYamlString<std::optional<std::string>>(yaml);
        REQUIRE(result->has_value());
        REQUIRE(*result == "");
    }
}
