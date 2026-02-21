#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format string edge cases round trip", "[yaml][block][string]")
{
    SECTION("my_string with leading spaces block round trip")
    {
        tst_struct obj;
        obj.my_string = "  leading spaces";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "  leading spaces");
    }

    SECTION("my_string with trailing spaces block round trip")
    {
        tst_struct obj;
        obj.my_string = "trailing spaces  ";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "trailing spaces  ");
    }

    SECTION("my_string with number-like content block round trip")
    {
        tst_struct obj;
        obj.my_string = "12345";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "12345");
    }

    SECTION("my_string with at-sign (YAML directive indicator) block round trip")
    {
        tst_struct obj;
        obj.my_string = "@username";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_string == "@username");
    }

    SECTION("my_list_std_string with url-like elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"http://example.com", "https://test.org/path?q=1"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it++ == "http://example.com");
        REQUIRE(*it++ == "https://test.org/path?q=1");
    }
}
