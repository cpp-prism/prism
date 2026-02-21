#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismYaml - zero sized container", "[yaml][zero_size][container]")
{
    SECTION("serialize empty vector<int> (block)")
    {
        std::vector<int> val = {};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        // 空 vector 序列化后是空字符串或包含换行符
        // 不检查特定内容，只检查反序列化功能
        auto result = prism::yaml::fromYamlString<std::vector<int>>(yaml);
        REQUIRE(result->empty());
    }

    SECTION("serialize empty list<string> (flow)")
    {
        std::list<std::string> val = {};
        std::string yaml = prism::yaml::toYamlStringFlow(val);
        REQUIRE(yaml == "[]");
    }

    SECTION("serialize empty map<string, int> (block)")
    {
        std::map<std::string, int> val = {};
        std::string yaml = prism::yaml::toYamlStringBlock(val);
        REQUIRE(yaml.empty());
    }

    SECTION("deserialize empty vector<int>")
    {
        std::string yaml = "";
        auto result = prism::yaml::fromYamlString<std::vector<int>>(yaml);
        REQUIRE(result->empty());
        REQUIRE(result->size() == 0);
    }

    SECTION("deserialize empty map<string, string>")
    {
        std::string yaml = "{}";
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::string>>(yaml);
        REQUIRE(result->empty());
        REQUIRE(result->size() == 0);
    }

    SECTION("serialize struct with empty container (block)")
    {
        tst_sub_struct obj;
        obj.my_vec_sp = {};
        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        REQUIRE(yaml.find("my_vec_sp:") != std::string::npos);
    }

    SECTION("deserialize struct with empty container")
    {
        std::string yaml = "my_vec_sp: []";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_vec_sp.size() == 0);
    }
}
