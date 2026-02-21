#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

// 单字段结构体 - 必须在全局作用域定义
struct single_field_int_struct_yaml {
    int only_field;
};
PRISM_FIELDS(single_field_int_struct_yaml, only_field)

struct single_field_string_struct_yaml {
    std::string only_field;
};
PRISM_FIELDS(single_field_string_struct_yaml, only_field)

struct single_field_bool_struct_yaml {
    bool only_field;
};
PRISM_FIELDS(single_field_bool_struct_yaml, only_field)

TEST_CASE("prismYaml - single field struct", "[yaml][single_field]")
{
    SECTION("serialize struct with single int field (block)")
    {
        single_field_int_struct_yaml obj;
        obj.only_field = 42;
        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        REQUIRE(yaml.find("only_field:") != std::string::npos);
        REQUIRE(yaml.find("42") != std::string::npos);
    }

    SECTION("deserialize struct with single int field")
    {
        std::string yaml = "only_field: 123";
        auto result = prism::yaml::fromYamlString<single_field_int_struct_yaml>(yaml);
        REQUIRE(result->only_field == 123);
    }

    SECTION("serialize struct with single string field (block)")
    {
        single_field_string_struct_yaml obj;
        obj.only_field = "single field test";
        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        REQUIRE(yaml.find("only_field:") != std::string::npos);
        REQUIRE(yaml.find("single field test") != std::string::npos);
    }

    SECTION("deserialize struct with single string field")
    {
        std::string yaml = "only_field: \"test value\"";
        auto result = prism::yaml::fromYamlString<single_field_string_struct_yaml>(yaml);
        REQUIRE(result->only_field == "test value");
    }

    SECTION("serialize struct with single bool field (flow)")
    {
        single_field_bool_struct_yaml obj;
        obj.only_field = true;
        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        REQUIRE(yaml.find("only_field:") != std::string::npos);
        REQUIRE(yaml.find("true") != std::string::npos);
    }

    SECTION("round trip single field struct")
    {
        single_field_int_struct_yaml original;
        original.only_field = 999;
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<single_field_int_struct_yaml>(yaml);
        REQUIRE(result->only_field == original.only_field);
    }
}
