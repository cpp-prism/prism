#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - YAML bool variants", "[yaml][bool][variants]")
{
    SECTION("true variants: true, True, TRUE, yes, Yes, YES, on, On, ON")
    {
        auto check_true = [](const std::string& val) {
            std::string yaml = "my_bool: " + val + "\n";
            auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
            return result->my_bool == true;
        };

        REQUIRE(check_true("true"));
        REQUIRE(check_true("True"));
        REQUIRE(check_true("TRUE"));
        REQUIRE(check_true("yes"));
        REQUIRE(check_true("Yes"));
        REQUIRE(check_true("YES"));
        REQUIRE(check_true("on"));
        REQUIRE(check_true("On"));
        REQUIRE(check_true("ON"));
    }

    SECTION("false variants: false, False, FALSE, no, No, NO, off, Off, OFF")
    {
        auto check_false = [](const std::string& val) {
            std::string yaml = "my_bool: " + val + "\n";
            auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
            return result->my_bool == false;
        };

        REQUIRE(check_false("false"));
        REQUIRE(check_false("False"));
        REQUIRE(check_false("FALSE"));
        REQUIRE(check_false("no"));
        REQUIRE(check_false("No"));
        REQUIRE(check_false("NO"));
        REQUIRE(check_false("off"));
        REQUIRE(check_false("Off"));
        REQUIRE(check_false("OFF"));
    }

    SECTION("bool in tst_struct flow round trip uses true/false")
    {
        tst_sub_struct original;
        original.my_bool = true;
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        REQUIRE(yaml.find("true") != std::string::npos);

        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_bool == true);
    }
}
