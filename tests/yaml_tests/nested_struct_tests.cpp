#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

// 3 层嵌套结构
struct level3_struct {
    int level3_value = 300;
    std::string level3_string = "level3";
};
PRISM_FIELDS(level3_struct, level3_value, level3_string)

struct level2_struct {
    int level2_value = 200;
    std::string level2_string = "level2";
    level3_struct level3;
};
PRISM_FIELDS(level2_struct, level2_value, level2_string, level3)

struct level1_struct {
    int level1_value = 100;
    std::string level1_string = "level1";
    level2_struct level2;
};
PRISM_FIELDS(level1_struct, level1_value, level1_string, level2)

TEST_CASE("prismYaml - 3 level nested struct", "[yaml][nested]")
{
    SECTION("serialize 3 level nested struct (block format)")
    {
        level1_struct obj;
        obj.level1_value = 111;
        obj.level1_string = "test1";
        obj.level2.level2_value = 222;
        obj.level2.level2_string = "test2";
        obj.level2.level3.level3_value = 333;
        obj.level2.level3.level3_string = "test3";

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        REQUIRE(yaml.find("level1_value:") != std::string::npos);
        REQUIRE(yaml.find("level2_value:") != std::string::npos);
        REQUIRE(yaml.find("level3_value:") != std::string::npos);
        REQUIRE(yaml.find("111") != std::string::npos);
        REQUIRE(yaml.find("222") != std::string::npos);
        REQUIRE(yaml.find("333") != std::string::npos);
    }

    SECTION("serialize 3 level nested struct (flow format)")
    {
        level1_struct obj;
        obj.level1_value = 111;
        obj.level1_string = "test1";
        obj.level2.level2_value = 222;
        obj.level2.level2_string = "test2";
        obj.level2.level3.level3_value = 333;
        obj.level2.level3.level3_string = "test3";

        std::string yaml = prism::yaml::toYamlStringFlow(obj, 2);
        REQUIRE(yaml.find("{") != std::string::npos);
        REQUIRE(yaml.find("}") != std::string::npos);
        REQUIRE(yaml.find("level1_value") != std::string::npos);
    }

    SECTION("deserialize 3 level nested struct")
    {
        // 使用 flow 格式：解析器支持嵌套 {...} 但不支持 block 缩进嵌套
        std::string yaml =
            "{level1_value: 111, level1_string: test1,"
            " level2: {level2_value: 222, level2_string: test2,"
            " level3: {level3_value: 333, level3_string: test3}}}";
        auto result = prism::yaml::fromYamlString<level1_struct>(yaml);
        REQUIRE(result->level1_value == 111);
        REQUIRE(result->level2.level2_value == 222);
        REQUIRE(result->level2.level3.level3_value == 333);
    }

    SECTION("round trip 3 level nested struct")
    {
        level1_struct obj;
        obj.level1_value = 111;
        obj.level1_string = "test1";
        obj.level2.level2_value = 222;
        obj.level2.level2_string = "test2";
        obj.level2.level3.level3_value = 333;
        obj.level2.level3.level3_string = "test3";

        // 使用 flow 格式保证 round trip 正确性
        std::string yaml = prism::yaml::toYamlStringFlow(obj, 2);
        auto result = prism::yaml::fromYamlString<level1_struct>(yaml);
        REQUIRE(result->level1_value == 111);
        REQUIRE(result->level2.level2_value == 222);
        REQUIRE(result->level2.level3.level3_value == 333);
    }
}
