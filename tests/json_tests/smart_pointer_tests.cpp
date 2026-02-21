#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismJson - smart pointer serialization", "[json][smart-pointer]")
{
    SECTION("serialize shared_ptr<int>")
    {
        std::shared_ptr<int> val = std::make_shared<int>(42);
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "42");
    }

    SECTION("serialize shared_ptr<std::string>")
    {
        std::shared_ptr<std::string> val = std::make_shared<std::string>("hello");
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "\"hello\"");
    }

    SECTION("serialize shared_ptr<tst_sub_struct>")
    {
        auto val = std::make_shared<tst_sub_struct>();
        val->my_int = 123;
        val->my_bool = true;
        val->my_string = "test";

        std::string json = prism::json::toJsonString(val);
        REQUIRE(json.find("\"my_int\":123") != std::string::npos);
        REQUIRE(json.find("\"my_bool\":true") != std::string::npos);
        REQUIRE(json.find("\"my_string\":\"test\"") != std::string::npos);
    }

    SECTION("serialize unique_ptr<int>")
    {
        std::unique_ptr<int> val = std::make_unique<int>(99);
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "99");
    }

    SECTION("serialize raw pointer")
    {
        int value = 777;
        int* ptr = &value;
        std::string json = prism::json::toJsonString(ptr);
        REQUIRE(json == "777");
    }

    SECTION("serialize nullptr")
    {
        std::shared_ptr<int> val = nullptr;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "null");
    }
}

TEST_CASE("prismJson - smart pointer deserialization", "[json][smart-pointer]")
{
    SECTION("deserialize shared_ptr<int>")
    {
        std::string json = "123";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 123);
    }

    SECTION("deserialize shared_ptr<std::string>")
    {
        std::string json = "\"hello\"";
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == "hello");
    }

    SECTION("deserialize shared_ptr<tst_sub_struct>")
    {
        std::string json = R"({"my_int":456,"my_bool":false,"my_string":"deserialize"})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "deserialize");
    }

    SECTION("deserialize unique_ptr<int>")
    {
        std::string json = "999";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 999);
    }

    SECTION("deserialize null value")
    {
        std::string json = "null";
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == 0);
    }
}

TEST_CASE("prismJson - smart pointer round trip", "[json][smart-pointer]")
{
    SECTION("round trip shared_ptr<int>")
    {
        std::shared_ptr<int> original = std::make_shared<int>(999);
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<int>(json);
        REQUIRE(*result == *original);
    }

    SECTION("round trip shared_ptr<tst_sub_struct>")
    {
        auto original = std::make_shared<tst_sub_struct>();
        original->my_int = 123;
        original->my_bool = true;
        original->my_float = 1.5f;
        original->my_double = 2.5;
        original->my_string = "test";

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_int == original->my_int);
        REQUIRE(result->my_bool == original->my_bool);
        REQUIRE(result->my_float == original->my_float);
        REQUIRE(result->my_double == original->my_double);
        REQUIRE(result->my_string == original->my_string);
    }
}

TEST_CASE("prismJson - smart pointer in struct", "[json][smart-pointer]")
{
    SECTION("serialize struct with shared_ptr field")
    {
        tst_struct obj;
        obj.my_sptr_int = std::make_shared<int>(42);

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"my_sptr_int\":42") != std::string::npos);
    }

    SECTION("deserialize struct with shared_ptr field")
    {
        std::string json = R"({"my_sptr_int":100})";
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(*result->my_sptr_int == 100);
    }

    SECTION("serialize nested shared_ptr in vector")
    {
        auto sub1 = std::make_shared<tst_sub_struct>();
        sub1->my_int = 1;
        auto sub2 = std::make_shared<tst_sub_struct>();
        sub2->my_int = 2;

        std::vector<std::shared_ptr<tst_sub_struct>> vec;
        vec.push_back(sub1);
        vec.push_back(sub2);

        std::string json = prism::json::toJsonString(vec);
        REQUIRE(json.find("\"my_int\":1") != std::string::npos);
        REQUIRE(json.find("\"my_int\":2") != std::string::npos);
    }
}
