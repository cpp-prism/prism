#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - partial deserialization", "[yaml][partial][struct]")
{
    SECTION("extra unknown fields are ignored")
    {
        // YAML with fields not in tst_sub_struct
        std::string yaml =
            "my_int: 5\n"
            "my_bool: true\n"
            "my_string: hello\n"
            "unknown_field: ignored\n"
            "extra_num: 999\n";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "hello");
    }

    SECTION("missing fields use default values")
    {
        // YAML with only some fields present
        std::string yaml = "my_int: 42\n";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == false);  // default
        REQUIRE(result->my_string == "");   // default
    }

    SECTION("empty YAML object uses all defaults")
    {
        std::string yaml = "{}\n";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "");
    }
}
