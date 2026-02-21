#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismYaml - smart pointer serialization", "[yaml][smart-pointer]")
{
    SECTION("serialize shared_ptr<int>")
    {
        std::shared_ptr<int> val = std::make_shared<int>(42);
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "42");
    }

    SECTION("serialize shared_ptr<std::string>")
    {
        std::shared_ptr<std::string> val = std::make_shared<std::string>("hello");
        std::string yaml = prism::yaml::toYamlString(val);
        // YAML 序列化字符串时不加引号
        REQUIRE(yaml == "hello");
    }

    SECTION("serialize shared_ptr<tst_sub_struct>")
    {
        auto val = std::make_shared<tst_sub_struct>();
        val->my_int = 123;
        val->my_bool = true;
        val->my_string = "test";

        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("my_int: 123") != std::string::npos);
        REQUIRE(yaml.find("my_bool: true") != std::string::npos);
        // YAML 字符串序列化不带引号
        REQUIRE(yaml.find("my_string: test") != std::string::npos);
    }

    SECTION("serialize unique_ptr<int>")
    {
        std::unique_ptr<int> val = std::make_unique<int>(99);
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "99");
    }

    SECTION("serialize raw pointer")
    {
        int value = 777;
        int* ptr = &value;
        std::string yaml = prism::yaml::toYamlString(ptr);
        REQUIRE(yaml == "777");
    }

    SECTION("serialize nullptr")
    {
        std::shared_ptr<int> val = nullptr;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "null");
    }
}

TEST_CASE("prismYaml - smart pointer deserialization", "[yaml][smart-pointer]")
{
    SECTION("deserialize shared_ptr<int>")
    {
        std::string yaml = "123";
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == 123);
    }

    SECTION("deserialize shared_ptr<std::string>")
    {
        std::string yaml = "\"hello\"";
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == "hello");
    }

    SECTION("deserialize shared_ptr<tst_sub_struct>")
    {
        std::string yaml = "my_int: 456\nmy_bool: false";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
    }

    SECTION("deserialize unique_ptr<int>")
    {
        std::string yaml = "999";
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == 999);
    }

    SECTION("deserialize null value")
    {
        std::string yaml = "null";
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == 0);
    }
}

TEST_CASE("prismYaml - smart pointer round trip", "[yaml][smart-pointer]")
{
    SECTION("round trip shared_ptr<int>")
    {
        std::shared_ptr<int> original = std::make_shared<int>(999);
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == *original);
    }

    SECTION("round trip shared_ptr<tst_sub_struct>")
    {
        auto original = std::make_shared<tst_sub_struct>();
        original->my_int = 123;
        original->my_bool = true;
        original->my_float = 1.5f;
        original->my_double = 2.5;

        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == original->my_int);
        REQUIRE(result->my_bool == original->my_bool);
        REQUIRE(result->my_float == original->my_float);
        REQUIRE(result->my_double == original->my_double);
    }
}

TEST_CASE("prismYaml - smart pointer in struct", "[yaml][smart-pointer]")
{
    SECTION("serialize struct with shared_ptr field")
    {
        tst_struct obj;
        obj.my_sptr_int = std::make_shared<int>(42);

        std::string yaml = prism::yaml::toYamlString(obj);
        REQUIRE(yaml.find("my_sptr_int: 42") != std::string::npos);
    }

    SECTION("deserialize struct with shared_ptr field")
    {
        std::string yaml = "my_sptr_int: 100";
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
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

        std::string yaml = prism::yaml::toYamlString(vec);
        REQUIRE(yaml.find("my_int: 1") != std::string::npos);
        REQUIRE(yaml.find("my_int: 2") != std::string::npos);
    }
}
