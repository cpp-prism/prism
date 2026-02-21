#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - optional<struct> round trip", "[yaml][optional][struct]")
{
    SECTION("tst_struct my_opt_struct with value block round trip")
    {
        tst_struct original;
        original.my_int = 7;
        original.my_opt_struct = tst_sub_struct{};
        original.my_opt_struct->my_int = 55;
        original.my_opt_struct->my_bool = true;
        original.my_opt_struct->my_string = "opt_val";

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 55);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "opt_val");
    }

    SECTION("tst_struct my_opt_struct null block round trip")
    {
        tst_struct original;
        original.my_int = 3;
        // my_opt_struct is nullopt by default

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("standalone optional<tst_sub_struct> with value flow round trip")
    {
        std::optional<tst_sub_struct> original = tst_sub_struct{};
        original->my_int = 88;
        original->my_string = "standalone_opt";

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::optional<tst_sub_struct>>(yaml);

        REQUIRE(result->has_value());
        REQUIRE((*result)->my_int == 88);
        REQUIRE((*result)->my_string == "standalone_opt");
    }

    SECTION("standalone optional<tst_sub_struct> null flow round trip")
    {
        std::optional<tst_sub_struct> original = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::optional<tst_sub_struct>>(yaml);

        REQUIRE(result->has_value() == false);
    }
}
