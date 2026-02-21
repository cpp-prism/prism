#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

// 定义带有忽略字段的测试结构体（YAML 版本）
struct yaml_ignore_test_struct
{
    int visible_field = 42;
    std::string visible_string = "keep";
    int ignored_field = 999;
    std::string ignored_string = "secret";
};
PRISM_FIELDS(yaml_ignore_test_struct, visible_field, visible_string, ignored_field, ignored_string)
PRISM_IGNORE_YAML_FIELD(yaml_ignore_test_struct, ignored_field)
PRISM_IGNORE_YAML_FIELD(yaml_ignore_test_struct, ignored_string)

TEST_CASE("prismYaml - PRISM_IGNORE_YAML_FIELD", "[yaml][attributes][ignore]")
{
    SECTION("ignored field not present in serialized YAML")
    {
        yaml_ignore_test_struct obj;
        obj.visible_field = 100;
        obj.visible_string = "visible";
        obj.ignored_field = 999;
        obj.ignored_string = "secret";

        std::string yaml = prism::yaml::toYamlString(obj);

        // 可见字段应存在
        REQUIRE(yaml.find("visible_field") != std::string::npos);
        REQUIRE(yaml.find("visible_string") != std::string::npos);
        REQUIRE(yaml.find("visible") != std::string::npos);

        // 忽略的字段不应出现在 YAML 中
        REQUIRE(yaml.find("ignored_field") == std::string::npos);
        REQUIRE(yaml.find("ignored_string") == std::string::npos);
        REQUIRE(yaml.find("secret") == std::string::npos);
    }

    SECTION("ignored field keeps C++ struct default value after deserialization")
    {
        // 提供包含忽略字段名的 YAML，反序列化后忽略字段保持 C++ 构造函数初始值
        std::string yaml = "visible_field: 200\nvisible_string: hello\nignored_field: 12345\nignored_string: should_not_appear";
        auto result = prism::yaml::fromYamlString<yaml_ignore_test_struct>(yaml);

        REQUIRE(result != nullptr);
        REQUIRE(result->visible_field == 200);
        REQUIRE(result->visible_string == "hello");
        // 忽略字段保持 C++ 结构体初始化默认值
        REQUIRE(result->ignored_field == 999);
        REQUIRE(result->ignored_string == "secret");
    }

    SECTION("round trip only preserves visible fields")
    {
        yaml_ignore_test_struct original;
        original.visible_field = 55;
        original.visible_string = "round_trip";
        original.ignored_field = 777;
        original.ignored_string = "hidden";

        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<yaml_ignore_test_struct>(yaml);

        // 可见字段保持一致
        REQUIRE(result->visible_field == original.visible_field);
        REQUIRE(result->visible_string == original.visible_string);
        // 忽略字段在 round trip 后，新对象保持 C++ 构造函数初始值
        REQUIRE(result->ignored_field == 999);
        REQUIRE(result->ignored_string == "secret");
    }
}
