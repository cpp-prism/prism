#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - container types", "[json][container]")
{
    SECTION("vector serialization")
    {
        std::vector<int> val = {1, 2, 3};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("[1,2,3]") != std::string::npos);
    }

    SECTION("list serialization")
    {
        std::list<int> val = {11, 22, 33};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("[11,22,33]") != std::string::npos);
    }

    SECTION("map serialization")
    {
        std::map<std::string, int> val = {{"a", 1}, {"b", 2}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"a\":1") != std::string::npos);
        REQUIRE(json.find("\"b\":2") != std::string::npos);
    }

    SECTION("nested vector serialization")
    {
        std::vector<std::vector<int>> val = {{1, 2}, {3, 4}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("[[1,2],[3,4]]") != std::string::npos);
    }
}

TEST_CASE("prismJson - list container tests", "[json][container][list]")
{
    SECTION("serialize list<int>")
    {
        std::list<int> val = {11, 22, 33};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("[11,22,33]") != std::string::npos);
    }

    SECTION("serialize list<string>")
    {
        std::list<std::string> val = {{"apple", "banana", "cherry"}};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("apple") != std::string::npos);
        REQUIRE(json.find("banana") != std::string::npos);
        REQUIRE(json.find("cherry") != std::string::npos);
    }

    SECTION("deserialize list<int>")
    {
        std::string json = "[100,200,300]";
        auto result = prism::json::fromJsonString<std::list<int>>(json);
        REQUIRE(result->size() == 3);
        std::vector<int> vec(result->begin(), result->end());
        REQUIRE(vec[0] == 100);
        REQUIRE(vec[1] == 200);
        REQUIRE(vec[2] == 300);
    }

    SECTION("deserialize list<string>")
    {
        std::string json = R"(["a","b","c"])";
        auto result = prism::json::fromJsonString<std::list<std::string>>(json);
        REQUIRE(result->size() == 3);
        std::vector<std::string> vec(result->begin(), result->end());
        REQUIRE(vec[0] == "a");
        REQUIRE(vec[1] == "b");
        REQUIRE(vec[2] == "c");
    }

    SECTION("serialize struct with list<int> (tst_struct::my_list_int)")
    {
        tst_struct obj;
        obj.my_list_int = {7, 8, 9};
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("my_list_int") != std::string::npos);
        REQUIRE(json.find("7") != std::string::npos);
    }

    SECTION("deserialize struct with list<int>")
    {
        std::string json = R"({"my_list_int":[1,2,3]})";
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_list_int.size() == 3);
        std::vector<int> vec(result->my_list_int.begin(), result->my_list_int.end());
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 3);
    }
}

TEST_CASE("prismJson - datetime types", "[json][datetime]")
{
    SECTION("serialize datetime")
    {
        auto datetime = initDatetime(2023, 6, 23, 12, 30, 45);
        std::string json = prism::json::toJsonString(datetime);
        REQUIRE(json.find("2023") != std::string::npos);
        REQUIRE(json.find("06") != std::string::npos);
        REQUIRE(json.find("23") != std::string::npos);
    }

    SECTION("deserialize datetime from ISO 8601 format")
    {
        std::string json = R"("2023-06-23T12:30:45Z")";
        auto result = prism::json::fromJsonString<std::chrono::system_clock::time_point>(json);
        std::time_t timestamp = std::chrono::system_clock::to_time_t(*result);
        // 验证年份
        std::tm* timeInfo = std::gmtime(&timestamp);
        REQUIRE(timeInfo->tm_year + 1900 == 2023);
        REQUIRE(timeInfo->tm_mon + 1 == 6);
        REQUIRE(timeInfo->tm_mday == 23);
    }

    SECTION("round trip datetime")
    {
        auto original = initDatetime(2023, 6, 23, 12, 30, 45);
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::chrono::system_clock::time_point>(json);
        std::time_t original_ts = std::chrono::system_clock::to_time_t(original);
        std::time_t result_ts = std::chrono::system_clock::to_time_t(*result);
        REQUIRE(original_ts == result_ts);
    }

    SECTION("serialize timestamp")
    {
        auto timestamp = initTimestamp(2023, 6, 23, 12, 30, 45);
        std::string json = prism::json::toJsonString(timestamp);
        // Timestamp should be a positive number around 2023
        std::cout << "timestamp json: " << json << std::endl;
        // Check that it's a valid timestamp (10 digits, starts with 1687)
        REQUIRE(json.length() >= 10);
        REQUIRE(json.find("1687") != std::string::npos);
    }

    SECTION("deserialize timestamp")
    {
        std::time_t original = initTimestamp(2023, 6, 23, 12, 30, 45);
        std::string json = std::to_string(original);
        auto result = prism::json::fromJsonString<std::time_t>(json);
        REQUIRE(*result == original);
    }
}
