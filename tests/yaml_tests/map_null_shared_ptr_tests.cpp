#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <map>

TEST_CASE("prismYaml - null shared_ptr in map", "[yaml][map][smart-pointer][null]")
{
    SECTION("map<string, shared_ptr<int>> with null value serialization")
    {
        std::map<std::string, std::shared_ptr<int>> m;
        m["alive"] = std::make_shared<int>(42);
        m["dead"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(m);
        REQUIRE(yaml.find("null") != std::string::npos);
        REQUIRE(yaml.find("42") != std::string::npos);
    }

    SECTION("map<string, shared_ptr<int>> with null value round trip")
    {
        std::map<std::string, std::shared_ptr<int>> original;
        original["alive"] = std::make_shared<int>(42);
        original["dead"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::shared_ptr<int>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["alive"] != nullptr);
        REQUIRE(*(*result)["alive"] == 42);
        REQUIRE((*result)["dead"] == nullptr);
    }

    SECTION("map<string, shared_ptr<int>> all null round trip")
    {
        std::map<std::string, std::shared_ptr<int>> original;
        original["a"] = nullptr;
        original["b"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::shared_ptr<int>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["a"] == nullptr);
        REQUIRE((*result)["b"] == nullptr);
    }

    SECTION("map<string, shared_ptr<int>> all non-null round trip")
    {
        std::map<std::string, std::shared_ptr<int>> original;
        original["x"] = std::make_shared<int>(10);
        original["y"] = std::make_shared<int>(20);
        original["z"] = std::make_shared<int>(30);

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::shared_ptr<int>>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE(*(*result)["x"] == 10);
        REQUIRE(*(*result)["y"] == 20);
        REQUIRE(*(*result)["z"] == 30);
    }
}
