#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

// 测试用结构体 - 必须在全局作用域定义
struct container_test_struct {
    std::unordered_set<int> ids;
    std::set<std::string> names;
    std::deque<float> values;
};
PRISM_FIELDS(container_test_struct, ids, names, values)

TEST_CASE("prismJson - expanded container types", "[json][container]")
{
    SECTION("serialize unordered_set<int>")
    {
        std::unordered_set<int> val = {1, 2, 3};
        std::string json = prism::json::toJsonString(val);
        // unordered_set 序列化为数组
        REQUIRE(json.find("1") != std::string::npos);
        REQUIRE(json.find("2") != std::string::npos);
        REQUIRE(json.find("3") != std::string::npos);
    }

    SECTION("serialize set<string>")
    {
        std::set<std::string> val = {"apple", "banana", "cherry"};
        std::string json = prism::json::toJsonString(val);
        // set 序列化为数组
        REQUIRE(json.find("apple") != std::string::npos);
        REQUIRE(json.find("banana") != std::string::npos);
        REQUIRE(json.find("cherry") != std::string::npos);
    }

    SECTION("serialize deque<float>")
    {
        std::deque<float> val = {1.1f, 2.2f, 3.3f};
        std::string json = prism::json::toJsonString(val);
        // deque 序列化为数组
        REQUIRE(json.find("1.1") != std::string::npos);
        REQUIRE(json.find("2.2") != std::string::npos);
        REQUIRE(json.find("3.3") != std::string::npos);
    }

    SECTION("deserialize unordered_set<int>")
    {
        std::string json = "[1,2,3]";
        auto result = prism::json::fromJsonString<std::unordered_set<int>>(json);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize set<string>")
    {
        std::string json = R"(["apple","banana"])";
        auto result = prism::json::fromJsonString<std::set<std::string>>(json);
        REQUIRE(result->size() == 2);
    }

    SECTION("serialize unordered_map<string, int>")
    {
        std::unordered_map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"a\"") != std::string::npos);
        REQUIRE(json.find("\"b\"") != std::string::npos);
        REQUIRE(json.find("1") != std::string::npos);
        REQUIRE(json.find("2") != std::string::npos);
    }

    SECTION("deserialize unordered_map<string, int>")
    {
        std::string json = R"({"x":10,"y":20})";
        auto result = prism::json::fromJsonString<std::unordered_map<std::string, int>>(json);
        REQUIRE(result->size() == 2);
    }

    SECTION("deserialize set<int>")
    {
        std::string json = "[1, 2, 3]";
        auto result = prism::json::fromJsonString<std::set<int>>(json);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize set<string>")
    {
        std::string json = R"(["a","b","c"])";
        auto result = prism::json::fromJsonString<std::set<std::string>>(json);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize deque<float>")
    {
        std::string json = "[1.1, 2.2, 3.3]";
        auto result = prism::json::fromJsonString<std::deque<float>>(json);
        REQUIRE(result->size() == 3);
    }

    SECTION("deserialize deque<int>")
    {
        std::string json = "[1, 2, 3]";
        auto result = prism::json::fromJsonString<std::deque<int>>(json);
        REQUIRE(result->size() == 3);
    }
}

TEST_CASE("prismJson - nested containers", "[json][container]")
{
    SECTION("serialize vector<set<int>>")
    {
        std::vector<std::set<int>> val = {{1, 2}, {3, 4}, {5, 6}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("[1,2]") != std::string::npos);
        REQUIRE(json.find("[3,4]") != std::string::npos);
        REQUIRE(json.find("[5,6]") != std::string::npos);
    }

    SECTION("deserialize vector<set<int>>")
    {
        std::string json = "[[1,2],[3,4]]";
        auto result = prism::json::fromJsonString<std::vector<std::set<int>>>(json);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].size() == 2);
        REQUIRE((*result)[1].size() == 2);
    }

    SECTION("serialize map<string, vector<int>>")
    {
        std::map<std::string, std::vector<int>> val = {
            {"a", {1, 2}},
            {"b", {3, 4}}
        };
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"a\"") != std::string::npos);
        REQUIRE(json.find("[1,2]") != std::string::npos);
    }
}

TEST_CASE("prismJson - container in struct", "[json][container]")
{
    SECTION("serialize struct with containers")
    {
        container_test_struct obj;
        obj.ids = {1, 2, 3};
        obj.names = {"alice", "bob"};
        obj.values = {1.1f, 2.2f};

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("1") != std::string::npos);
        REQUIRE(json.find("alice") != std::string::npos);
        REQUIRE(json.find("1.1") != std::string::npos);
    }

    SECTION("deserialize struct with containers")
    {
        std::string json = R"({"ids":[1,2,3],"names":["alice","bob"],"values":[1.1,2.2]})";
        auto result = prism::json::fromJsonString<container_test_struct>(json);
        REQUIRE(result->ids.size() == 3);
        REQUIRE(result->names.size() == 2);
        REQUIRE(result->values.size() == 2);
    }
}

TEST_CASE("prismJson - complex nested containers", "[json][container]")
{
    SECTION("serialize vector<map<string, int>>")
    {
        std::vector<std::map<std::string, int>> val = {
            {{"a", 1}, {"b", 2}},
            {{"c", 3}, {"d", 4}}
        };
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("{\"a\":1") != std::string::npos);
        REQUIRE(json.find("{\"c\":3") != std::string::npos);
    }

    SECTION("deserialize vector<map<string, int>>")
    {
        std::string json = R"([{"a":1,"b":2},{"c":3,"d":4}])";
        auto result = prism::json::fromJsonString<std::vector<std::map<std::string, int>>>(json);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].size() == 2);
        REQUIRE((*result)[1].size() == 2);
    }

    SECTION("serialize map<string, vector<set<int>>>")
    {
        std::map<std::string, std::vector<std::set<int>>> val = {
            {"a", {{1, 2}, {3, 4}}},
            {"b", {{5, 6}, {7, 8}}}
        };
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"a\"") != std::string::npos);
        REQUIRE(json.find("[1,2]") != std::string::npos);
    }

    SECTION("deserialize map<string, vector<set<int>>>")
    {
        std::string json = R"({"a":[[1,2],[3,4]],"b":[[5,6],[7,8]]})";
        auto result = prism::json::fromJsonString<std::map<std::string, std::vector<std::set<int>>>>(json);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)["a"].size() == 2);
    }
}
