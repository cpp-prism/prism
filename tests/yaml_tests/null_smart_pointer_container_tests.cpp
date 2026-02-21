#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

TEST_CASE("prismYaml - null shared_ptr in containers", "[yaml][container][smart-pointer][null]")
{
    SECTION("vector of shared_ptr with mixed null and non-null elements serialization")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(std::make_shared<int>(1));
        vec.push_back(nullptr);
        vec.push_back(std::make_shared<int>(3));

        std::string yaml = prism::yaml::toYamlString(vec);
        // null 元素应序列化为 null
        REQUIRE(yaml.find("null") != std::string::npos);
        REQUIRE(yaml.find("1") != std::string::npos);
        REQUIRE(yaml.find("3") != std::string::npos);
    }

    SECTION("vector of shared_ptr round trip with null elements")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(std::make_shared<int>(10));
        vec.push_back(nullptr);
        vec.push_back(std::make_shared<int>(30));

        // 使用 flow 格式保证 round trip 正确性
        std::string yaml = prism::yaml::toYamlStringFlow(vec);
        auto result = prism::yaml::fromYamlString<std::vector<std::shared_ptr<int>>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] != nullptr);
        REQUIRE(*(*result)[0] == 10);
        REQUIRE((*result)[1] == nullptr);
        REQUIRE((*result)[2] != nullptr);
        REQUIRE(*(*result)[2] == 30);
    }

    SECTION("all null shared_ptr vector round trip")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(nullptr);
        vec.push_back(nullptr);

        std::string yaml = prism::yaml::toYamlStringFlow(vec);
        auto result = prism::yaml::fromYamlString<std::vector<std::shared_ptr<int>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0] == nullptr);
        REQUIRE((*result)[1] == nullptr);
    }
}
