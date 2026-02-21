#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

// 包含 shared_ptr 容器的测试结构体
struct json_ptr_container_struct
{
    std::vector<std::shared_ptr<tst_sub_struct>> items;
};
PRISM_FIELDS(json_ptr_container_struct, items)

TEST_CASE("prismJson - null shared_ptr in containers", "[json][container][smart-pointer][null]")
{
    SECTION("vector of shared_ptr with mixed null and non-null elements serialization")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(std::make_shared<int>(1));
        vec.push_back(nullptr);
        vec.push_back(std::make_shared<int>(3));

        std::string json = prism::json::toJsonString(vec);
        // null 元素应序列化为 null
        REQUIRE(json.find("null") != std::string::npos);
        REQUIRE(json.find("1") != std::string::npos);
        REQUIRE(json.find("3") != std::string::npos);
    }

    SECTION("vector of shared_ptr round trip with null elements")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(std::make_shared<int>(10));
        vec.push_back(nullptr);
        vec.push_back(std::make_shared<int>(30));

        std::string json = prism::json::toJsonString(vec);
        auto result = prism::json::fromJsonString<std::vector<std::shared_ptr<int>>>(json);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] != nullptr);
        REQUIRE(*(*result)[0] == 10);
        REQUIRE((*result)[1] == nullptr);
        REQUIRE((*result)[2] != nullptr);
        REQUIRE(*(*result)[2] == 30);
    }

    SECTION("all null shared_ptr vector serialization")
    {
        std::vector<std::shared_ptr<int>> vec;
        vec.push_back(nullptr);
        vec.push_back(nullptr);

        std::string json = prism::json::toJsonString(vec);
        auto result = prism::json::fromJsonString<std::vector<std::shared_ptr<int>>>(json);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0] == nullptr);
        REQUIRE((*result)[1] == nullptr);
    }

    SECTION("map with null shared_ptr values round trip")
    {
        std::map<std::string, std::shared_ptr<int>> m;
        m["alive"] = std::make_shared<int>(42);
        m["dead"] = nullptr;

        std::string json = prism::json::toJsonString(m);
        auto result = prism::json::fromJsonString<std::map<std::string, std::shared_ptr<int>>>(json);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)["alive"] != nullptr);
        REQUIRE(*(*result)["alive"] == 42);
        REQUIRE((*result)["dead"] == nullptr);
    }
}
