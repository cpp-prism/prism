#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - empty string", "[yaml][string][empty]")
{
    SECTION("serialize empty string")
    {
        std::string empty = "";
        std::string yaml = prism::yaml::toYamlString(empty);
        REQUIRE(yaml == "\"\"");
    }

    SECTION("deserialize empty string from yaml")
    {
        std::string yaml = "\"\"";
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == "");
    }

    SECTION("round trip empty string")
    {
        std::string original = "";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("serialize optional empty string")
    {
        std::optional<std::string> opt = std::optional<std::string>("");
        std::string yaml = prism::yaml::toYamlString(opt);
        REQUIRE(yaml.find("\"\"") != std::string::npos);
    }
}
