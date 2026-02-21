#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

// 测试用结构体，包含单个元素容器
struct single_element_test_struct {
    std::list<int> my_list_int;
    std::vector<std::string> my_vec_str;
};
PRISM_FIELDS(single_element_test_struct, my_list_int, my_vec_str)

TEST_CASE("prismJson - single element container", "[json][single_element][container]")
{
    SECTION("serialize vector<int> with one element")
    {
        std::vector<int> val = {42};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "[42]");
    }

    SECTION("serialize vector<string> with one element")
    {
        std::vector<std::string> val = {"single"};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == R"(["single"])");
    }

    SECTION("serialize list<int> with one element")
    {
        std::list<int> val = {99};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "[99]");
    }

    SECTION("serialize map<string, int> with one element")
    {
        std::map<std::string, int> val = {{"key", 123}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"key\":123") != std::string::npos);
    }

    SECTION("deserialize vector<int> with single element")
    {
        std::string json = "[100]";
        auto result = prism::json::fromJsonString<std::vector<int>>(json);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)[0] == 100);
    }

    SECTION("deserialize map<string, int> with single element")
    {
        std::string json = R"({"name":"value"})";
        auto result = prism::json::fromJsonString<std::map<std::string, std::string>>(json);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)["name"] == "value");
    }

    SECTION("serialize struct with single element container")
    {
        single_element_test_struct obj;
        obj.my_list_int = {777};
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"my_list_int\":[777]") != std::string::npos);
    }

    SECTION("deserialize struct with single element container")
    {
        std::string json = R"({"my_list_int":[42]})";
        auto result = prism::json::fromJsonString<single_element_test_struct>(json);
        REQUIRE(result->my_list_int.size() == 1);
        REQUIRE(result->my_list_int.front() == 42);
    }

    SECTION("round trip single element vector")
    {
        std::vector<int> original = {123};
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::vector<int>>(json);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)[0] == original[0]);
    }

    SECTION("round trip single element map")
    {
        std::map<std::string, int> original = {{"test", 456}};
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::map<std::string, int>>(json);
        REQUIRE(result->size() == 1);
        REQUIRE((*result)["test"] == original["test"]);
    }
}
