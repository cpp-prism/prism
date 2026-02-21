#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

// 单字段结构体 - 必须在全局作用域定义
struct single_field_int_struct {
    int only_field;
};
PRISM_FIELDS(single_field_int_struct, only_field)

struct single_field_string_struct {
    std::string only_field;
};
PRISM_FIELDS(single_field_string_struct, only_field)

struct single_field_bool_struct {
    bool only_field;
};
PRISM_FIELDS(single_field_bool_struct, only_field)

TEST_CASE("prismJson - single field struct", "[json][single_field]")
{
    SECTION("serialize struct with single int field")
    {
        single_field_int_struct obj;
        obj.only_field = 42;
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"only_field\":42") != std::string::npos);
    }

    SECTION("deserialize struct with single int field")
    {
        std::string json = R"({"only_field":123})";
        auto result = prism::json::fromJsonString<single_field_int_struct>(json);
        REQUIRE(result->only_field == 123);
    }

    SECTION("serialize struct with single string field")
    {
        single_field_string_struct obj;
        obj.only_field = "single field test";
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"only_field\":\"single field test\"") != std::string::npos);
    }

    SECTION("deserialize struct with single string field")
    {
        std::string json = R"({"only_field":"test value"})";
        auto result = prism::json::fromJsonString<single_field_string_struct>(json);
        REQUIRE(result->only_field == "test value");
    }

    SECTION("serialize struct with single bool field")
    {
        single_field_bool_struct obj;
        obj.only_field = true;
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"only_field\":true") != std::string::npos);
    }

    SECTION("round trip single field struct")
    {
        single_field_int_struct original;
        original.only_field = 999;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<single_field_int_struct>(json);
        REQUIRE(result->only_field == original.only_field);
    }
}
