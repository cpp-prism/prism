#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - optional<int> serialization", "[yaml][optional]")
{
    SECTION("serialize optional<int> with value")
    {
        std::optional<int> val = 42;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "42");
    }

    SECTION("serialize optional<int> empty")
    {
        std::optional<int> val = std::nullopt;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "null");
    }

    SECTION("deserialize optional<int> from yaml")
    {
        SECTION("with value")
        {
            std::string yaml = "123";
            auto result = prism::yaml::fromYamlString<std::optional<int>>(yaml);
            REQUIRE(result->has_value());
            REQUIRE(**result == 123);
        }

        SECTION("null value")
        {
            std::string yaml = "null";
            auto result = prism::yaml::fromYamlString<std::optional<int>>(yaml);
            REQUIRE(!result->has_value());
        }
    }

    SECTION("round trip optional<int>")
    {
        std::optional<int> original = 999;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::optional<int>>(yaml);
        REQUIRE(result->has_value());
        REQUIRE(**result == original.value());
    }

    SECTION("round trip optional<string>")
    {
        std::optional<std::string> original = "test optional string";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::optional<std::string>>(yaml);
        REQUIRE(result->has_value());
        REQUIRE(**result == original.value());
    }

    SECTION("optional in struct")
    {
        tst_sub_struct obj;
        obj.my_opt_int = 777;
        std::string yaml = prism::yaml::toYamlString(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 777);
    }

    SECTION("nested optional")
    {
        std::optional<std::vector<std::optional<int>>> nested = std::vector<std::optional<int>>{1, std::nullopt, 3};
        std::string yaml = prism::yaml::toYamlString(nested);
        auto result = prism::yaml::fromYamlString<std::optional<std::vector<std::optional<int>>>>(yaml);
        REQUIRE(result->has_value());
        auto& vec = result->value();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0].has_value());
        REQUIRE(vec[0].value() == 1);
        REQUIRE(!vec[1].has_value());
        REQUIRE(vec[2].has_value());
        REQUIRE(vec[2].value() == 3);
    }
}
