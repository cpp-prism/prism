#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
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

TEST_CASE("prismJson - 3 level nested struct", "[json][nested]")
{
    SECTION("serialize 3 level nested struct")
    {
        level1_struct obj;
        obj.level1_value = 111;
        obj.level1_string = "test1";
        obj.level2.level2_value = 222;
        obj.level2.level2_string = "test2";
        obj.level2.level3.level3_value = 333;
        obj.level2.level3.level3_string = "test3";

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("level1_value") != std::string::npos);
        REQUIRE(json.find("level2_value") != std::string::npos);
        REQUIRE(json.find("level3_value") != std::string::npos);
        REQUIRE(json.find("111") != std::string::npos);
        REQUIRE(json.find("222") != std::string::npos);
        REQUIRE(json.find("333") != std::string::npos);
    }

    SECTION("deserialize 3 level nested struct")
    {
        // 注意：JSON 中 level2 应该是一个嵌套对象
        std::string json = R"({
            "level1_value": 1001,
            "level1_string": "l1",
            "level2": {
                "level2_value": 2002,
                "level2_string": "l2",
                "level3": {
                    "level3_value": 3003,
                    "level3_string": "l3"
                }
            }
        })";

        auto result = prism::json::fromJsonString<level1_struct>(json);
        REQUIRE(result->level1_value == 1001);
        REQUIRE(result->level1_string == "l1");
        REQUIRE(result->level2.level2_value == 2002);
        REQUIRE(result->level2.level2_string == "l2");
        REQUIRE(result->level2.level3.level3_value == 3003);
        REQUIRE(result->level2.level3.level3_string == "l3");
    }

    SECTION("round trip 3 level nested struct")
    {
        level1_struct original;
        original.level1_value = 777;
        original.level1_string = "round";
        original.level2.level2_value = 888;
        original.level2.level2_string = "trip";
        original.level2.level3.level3_value = 999;
        original.level2.level3.level3_string = "nested";

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<level1_struct>(json);

        REQUIRE(result->level1_value == original.level1_value);
        REQUIRE(result->level1_string == original.level1_string);
        REQUIRE(result->level2.level2_value == original.level2.level2_value);
        REQUIRE(result->level2.level2_string == original.level2.level2_string);
        REQUIRE(result->level2.level3.level3_value == original.level2.level3.level3_value);
        REQUIRE(result->level2.level3.level3_string == original.level2.level3.level3_string);
    }
}
