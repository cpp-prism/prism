#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - YAML null variants", "[yaml][null][variants]")
{
    SECTION("optional<string>: null variants produce nullopt")
    {
        auto check_null = [](const std::string& val) {
            std::string yaml = "my_opt_str: " + val + "\n";
            auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
            return !result->my_opt_str.has_value();
        };

        REQUIRE(check_null("null"));
        REQUIRE(check_null("Null"));
        REQUIRE(check_null("NULL"));
        REQUIRE(check_null("~"));
    }

    SECTION("optional<int>: null variants produce nullopt")
    {
        auto check_null_int = [](const std::string& val) {
            std::string yaml = "my_opt_int: " + val + "\n";
            auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
            return !result->my_opt_int.has_value();
        };

        REQUIRE(check_null_int("null"));
        REQUIRE(check_null_int("Null"));
        REQUIRE(check_null_int("NULL"));
        REQUIRE(check_null_int("~"));
    }

    SECTION("shared_ptr: null variants produce nullptr")
    {
        auto check_null_ptr = [](const std::string& val) {
            std::string yaml = "my_shared_sub: " + val + "\n";
            auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
            return result->my_shared_sub == nullptr;
        };

        REQUIRE(check_null_ptr("null"));
        REQUIRE(check_null_ptr("Null"));
        REQUIRE(check_null_ptr("NULL"));
        REQUIRE(check_null_ptr("~"));
    }

    SECTION("empty value field leaves optional as nullopt")
    {
        // When value is omitted (empty inline), optional stays nullopt
        std::string yaml = "my_int: 5\nmy_opt_str:\nmy_bool: true\n";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 5);
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_bool == true);
    }
}
