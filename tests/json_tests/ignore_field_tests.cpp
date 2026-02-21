#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

// 定义带有忽略字段的测试结构体
struct json_ignore_test_struct
{
    int visible_field = 42;
    std::string visible_string = "keep";
    int ignored_field = 999;
    std::string ignored_string = "secret";
};
PRISM_FIELDS(json_ignore_test_struct, visible_field, visible_string, ignored_field, ignored_string)
PRISM_IGNORE_JSON_FIELD(json_ignore_test_struct, ignored_field)
PRISM_IGNORE_JSON_FIELD(json_ignore_test_struct, ignored_string)

TEST_CASE("prismJson - PRISM_IGNORE_JSON_FIELD", "[json][attributes][ignore]")
{
    SECTION("ignored field not present in serialized JSON")
    {
        json_ignore_test_struct obj;
        obj.visible_field = 100;
        obj.visible_string = "visible";
        obj.ignored_field = 999;
        obj.ignored_string = "secret";

        std::string json = prism::json::toJsonString(obj);

        // 可见字段应存在
        REQUIRE(json.find("visible_field") != std::string::npos);
        REQUIRE(json.find("visible_string") != std::string::npos);
        REQUIRE(json.find("visible") != std::string::npos);

        // 忽略的字段不应出现在 JSON 中
        REQUIRE(json.find("ignored_field") == std::string::npos);
        REQUIRE(json.find("ignored_string") == std::string::npos);
        REQUIRE(json.find("secret") == std::string::npos);
    }

    SECTION("ignored field keeps C++ struct default value after deserialization")
    {
        // 提供包含忽略字段名的 JSON，反序列化后忽略字段保持 C++ 构造函数初始值（999 和 "secret"）
        // PRISM_IGNORE_JSON_FIELD 在反序列化时也跳过该字段，不会写入 JSON 中的值
        std::string json = R"({"visible_field":200,"visible_string":"hello","ignored_field":12345,"ignored_string":"should_not_appear"})";
        auto result = prism::json::fromJsonString<json_ignore_test_struct>(json);

        REQUIRE(result != nullptr);
        REQUIRE(result->visible_field == 200);
        REQUIRE(result->visible_string == "hello");
        // 忽略字段保持 C++ 结构体初始化默认值，而不是 JSON 中的值
        REQUIRE(result->ignored_field == 999);
        REQUIRE(result->ignored_string == "secret");
    }

    SECTION("round trip only preserves visible fields")
    {
        json_ignore_test_struct original;
        original.visible_field = 55;
        original.visible_string = "round_trip";
        original.ignored_field = 777;  // 被忽略，不会出现在 JSON 中
        original.ignored_string = "hidden";  // 被忽略

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<json_ignore_test_struct>(json);

        // 可见字段保持一致
        REQUIRE(result->visible_field == original.visible_field);
        REQUIRE(result->visible_string == original.visible_string);
        // 忽略字段在 round trip 后，新对象保持 C++ 构造函数初始值（999 和 "secret"）
        REQUIRE(result->ignored_field == 999);
        REQUIRE(result->ignored_string == "secret");
    }
}
