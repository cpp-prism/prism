#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include <string>

TEST_CASE("prismYaml - unordered_map round trip", "[yaml][unordered_map]")
{
    SECTION("unordered_map<string, int> round trip")
    {
        std::unordered_map<std::string, int> original;
        original["alpha"] = 1;
        original["beta"] = 2;
        original["gamma"] = 3;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, int>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)["alpha"] == 1);
        REQUIRE((*result)["beta"] == 2);
        REQUIRE((*result)["gamma"] == 3);
    }

    SECTION("unordered_map<string, string> round trip")
    {
        std::unordered_map<std::string, std::string> original;
        original["key1"] = "val1";
        original["key2"] = "val2";

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, std::string>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["key1"] == "val1");
        REQUIRE((*result)["key2"] == "val2");
    }

    SECTION("unordered_map<string, double> round trip")
    {
        std::unordered_map<std::string, double> original;
        original["pi"] = 3.14;
        original["e"] = 2.71;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, double>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE(std::abs((*result)["pi"] - 3.14) < 0.001);
        REQUIRE(std::abs((*result)["e"] - 2.71) < 0.001);
    }

    SECTION("unordered_map<string, tst_sub_struct> round trip")
    {
        std::unordered_map<std::string, tst_sub_struct> original;
        tst_sub_struct s1;
        s1.my_int = 100;
        s1.my_string = "first";
        original["s1"] = s1;
        tst_sub_struct s2;
        s2.my_int = 200;
        s2.my_string = "second";
        original["s2"] = s2;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["s1"].my_int == 100);
        REQUIRE((*result)["s1"].my_string == "first");
        REQUIRE((*result)["s2"].my_int == 200);
        REQUIRE((*result)["s2"].my_string == "second");
    }
}
