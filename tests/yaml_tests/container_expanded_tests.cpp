#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

// 测试用结构体 - 必须在全局作用域定义
struct yaml_container_test_struct {
    std::unordered_set<int> ids;
    std::set<std::string> names;
    std::deque<float> values;
};
PRISM_FIELDS(yaml_container_test_struct, ids, names, values)

TEST_CASE("prismYaml - expanded container types", "[yaml][container]")
{
    SECTION("serialize set<int> (block)")
    {
        std::set<int> val = {1, 2, 3};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        REQUIRE(yaml.find("1") != std::string::npos);
        REQUIRE(yaml.find("2") != std::string::npos);
        REQUIRE(yaml.find("3") != std::string::npos);
    }

    SECTION("serialize set<string> (flow)")
    {
        std::set<std::string> val = {"apple", "banana", "cherry"};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("apple") != std::string::npos);
        REQUIRE(yaml.find("banana") != std::string::npos);
        REQUIRE(yaml.find("cherry") != std::string::npos);
    }

    SECTION("serialize deque<int> (block)")
    {
        std::deque<int> val = {10, 20, 30};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        REQUIRE(yaml.find("10") != std::string::npos);
        REQUIRE(yaml.find("20") != std::string::npos);
        REQUIRE(yaml.find("30") != std::string::npos);
    }

    SECTION("serialize deque<float> (flow)")
    {
        // 使用精确的二进制浮点数，避免精度问题
        std::deque<float> val = {1.5f, 2.5f, 3.5f};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("1.5") != std::string::npos);
        REQUIRE(yaml.find("2.5") != std::string::npos);
        REQUIRE(yaml.find("3.5") != std::string::npos);
    }

    SECTION("serialize unordered_set<int>")
    {
        std::unordered_set<int> val = {1, 2, 3};
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("1") != std::string::npos);
        REQUIRE(yaml.find("2") != std::string::npos);
        REQUIRE(yaml.find("3") != std::string::npos);
    }

    SECTION("deserialize unordered_set<int>")
    {
        std::string yaml = "[1, 2, 3]";
        auto result = prism::yaml::fromYamlString<std::unordered_set<int>>(yaml);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize unordered_map<string, int>")
    {
        std::string yaml = "{a: 1, b: 2}";
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, int>>(yaml);
        REQUIRE(result->size() == 2);
    }

    SECTION("round trip set<int>")
    {
        std::set<int> original = {5, 10, 15};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::set<int>>(yaml);
        REQUIRE(result->size() == 3);
        REQUIRE(result->count(5) == 1);
        REQUIRE(result->count(10) == 1);
        REQUIRE(result->count(15) == 1);
    }

    SECTION("round trip deque<int>")
    {
        std::deque<int> original = {7, 8, 9};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::deque<int>>(yaml);
        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] == 7);
        REQUIRE((*result)[1] == 8);
        REQUIRE((*result)[2] == 9);
    }
}

TEST_CASE("prismYaml - nested containers", "[yaml][container]")
{
    SECTION("serialize vector<set<int>> (flow)")
    {
        std::vector<std::set<int>> val = {{1, 2}, {3, 4}, {5, 6}};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("[1, 2]") != std::string::npos);
        REQUIRE(yaml.find("[3, 4]") != std::string::npos);
        REQUIRE(yaml.find("[5, 6]") != std::string::npos);
    }

    SECTION("deserialize vector<set<int>>")
    {
        std::string yaml = "[[1, 2], [3, 4]]";
        auto result = prism::yaml::fromYamlString<std::vector<std::set<int>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].size() == 2);
        REQUIRE((*result)[1].size() == 2);
    }

    SECTION("serialize map<string, vector<int>> (flow)")
    {
        std::map<std::string, std::vector<int>> val = {{"a", {1, 2}}, {"b", {3, 4}}};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("a:") != std::string::npos);
        REQUIRE(yaml.find("[1, 2]") != std::string::npos);
    }

    SECTION("deserialize map<string, vector<int>>")
    {
        std::string yaml = "{a: [1, 2], b: [3, 4]}";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::vector<int>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)["a"].size() == 2);
        REQUIRE((*result)["b"].size() == 2);
    }

    SECTION("round trip vector<set<int>>")
    {
        std::vector<std::set<int>> original = {{5, 10}, {15, 20}};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<std::set<int>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].count(5) == 1);
        REQUIRE((*result)[0].count(10) == 1);
        REQUIRE((*result)[1].count(15) == 1);
        REQUIRE((*result)[1].count(20) == 1);
    }
}

TEST_CASE("prismYaml - container in struct", "[yaml][container]")
{
    SECTION("serialize struct with containers (block)")
    {
        yaml_container_test_struct obj;
        obj.ids = {1, 2, 3};
        obj.names = {"alice", "bob"};
        obj.values = {1.5f, 2.5f};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        REQUIRE(yaml.find("ids:") != std::string::npos);
        REQUIRE(yaml.find("names:") != std::string::npos);
        REQUIRE(yaml.find("alice") != std::string::npos);
        REQUIRE(yaml.find("1.5") != std::string::npos);
    }

    SECTION("serialize struct with containers (flow)")
    {
        yaml_container_test_struct obj;
        obj.ids = {42};
        obj.names = {"test"};
        obj.values = {3.5f};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        REQUIRE(yaml.find("{") != std::string::npos);
        REQUIRE(yaml.find("42") != std::string::npos);
        REQUIRE(yaml.find("test") != std::string::npos);
    }

    SECTION("deserialize struct with containers")
    {
        yaml_container_test_struct obj;
        obj.ids = {1, 2, 3};
        obj.names = {"alice", "bob"};
        obj.values = {1.5f, 2.5f};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<yaml_container_test_struct>(yaml);
        REQUIRE(result->ids.size() == 3);
        REQUIRE(result->names.size() == 2);
        REQUIRE(result->values.size() == 2);
    }
}

TEST_CASE("prismYaml - complex nested containers", "[yaml][container]")
{
    SECTION("serialize vector<map<string, int>> (flow)")
    {
        std::vector<std::map<std::string, int>> val = {
            {{"a", 1}, {"b", 2}},
            {{"c", 3}, {"d", 4}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("{a: 1") != std::string::npos);
        REQUIRE(yaml.find("{c: 3") != std::string::npos);
    }

    SECTION("deserialize vector<map<string, int>>")
    {
        std::string yaml = "[{a: 1, b: 2}, {c: 3, d: 4}]";
        auto result = prism::yaml::fromYamlString<std::vector<std::map<std::string, int>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].size() == 2);
        REQUIRE((*result)[1].size() == 2);
    }

    SECTION("serialize map<string, vector<set<int>>> (flow)")
    {
        std::map<std::string, std::vector<std::set<int>>> val = {
            {"a", {{1, 2}, {3, 4}}},
            {"b", {{5, 6}}}
        };
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("a:") != std::string::npos);
        REQUIRE(yaml.find("[1, 2]") != std::string::npos);
    }

    SECTION("deserialize map<string, vector<set<int>>>")
    {
        std::string yaml = "{a: [[1, 2], [3, 4]], b: [[5, 6]]}";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::vector<std::set<int>>>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)["a"].size() == 2);
        REQUIRE((*result)["b"].size() == 1);
    }
}
