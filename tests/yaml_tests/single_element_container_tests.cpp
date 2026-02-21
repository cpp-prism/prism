#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

// 测试用结构体，包含单个元素容器
struct single_element_test_struct_yaml {
    std::list<int> my_list_int;
    std::vector<std::string> my_vec_str;
};
PRISM_FIELDS(single_element_test_struct_yaml, my_list_int, my_vec_str)

TEST_CASE("prismYaml - single element container", "[yaml][single_element][container]")
{
    SECTION("serialize vector<int> with one element (block)")
    {
        std::vector<int> val = {42};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        REQUIRE(yaml.find("- 42") != std::string::npos);
    }

    SECTION("serialize vector<string> with one element (flow)")
    {
        std::vector<std::string> val = {"single"};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("single") != std::string::npos);
    }

    SECTION("serialize list<int> with one element (block)")
    {
        std::list<int> val = {99};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        REQUIRE(yaml.find("- 99") != std::string::npos);
    }

    SECTION("serialize map<string, int> with one element (flow)")
    {
        std::map<std::string, int> val = {{"key", 123}};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml.find("key:") != std::string::npos);
        REQUIRE(yaml.find("123") != std::string::npos);
    }

    SECTION("deserialize vector<int> with single element")
    {
        std::string yaml = "- 100";
        auto result = prism::yaml::fromYamlString<std::vector<int>>(yaml);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)[0] == 100);
    }

    SECTION("deserialize map<string, string> with single element")
    {
        std::string yaml = "{name: value}";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::string>>(yaml);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)["name"] == "value");
    }

    SECTION("serialize struct with single element container (block)")
    {
        single_element_test_struct_yaml obj;
        obj.my_list_int = {777};
        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        REQUIRE(yaml.find("my_list_int:") != std::string::npos);
        REQUIRE(yaml.find("777") != std::string::npos);
    }

    SECTION("deserialize struct with single element container")
    {
        std::string yaml = "my_list_int: [42]";
        auto result = prism::yaml::fromYamlString<single_element_test_struct_yaml>(yaml);
        REQUIRE(result->my_list_int.size() == 1);
        REQUIRE(result->my_list_int.front() == 42);
    }

    SECTION("round trip single element vector")
    {
        std::vector<int> original = {123};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<int>>(yaml);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)[0] == original[0]);
    }

    SECTION("round trip single element map")
    {
        std::map<std::string, int> original = {{"test", 456}};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, int>>(yaml);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)["test"] == original["test"]);
    }
}
