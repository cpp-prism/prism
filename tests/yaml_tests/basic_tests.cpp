#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - basic types", "[yaml]")
{
    SECTION("serialize int")
    {
        int val = 42;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "42");
    }

    SECTION("deserialize int from yaml")
    {
        std::string yaml = "123";
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == 123);
    }

    SECTION("serialize double")
    {
        double val = 3.14;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("3.14") != std::string::npos);
    }

    SECTION("serialize bool")
    {
        bool val = true;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "true");
    }

    SECTION("deserialize bool from yaml")
    {
        std::string yaml = "false";
        auto result = prism::yaml::fromYamlString<bool>(yaml);
        REQUIRE(*result == false);
    }

    SECTION("serialize negative int")
    {
        int val = -42;
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml == "-42");
    }

    SECTION("deserialize negative float from yaml")
    {
        std::string yaml = "-3.14";
        auto result = prism::yaml::fromYamlString<double>(yaml);
        REQUIRE(std::abs(*result - (-3.14)) < 0.001);
    }
}

TEST_CASE("prismYaml - round trip", "[yaml]")
{
    SECTION("round trip int")
    {
        int original = 999;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<int>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("round trip string")
    {
        std::string original = "test round trip";
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<std::string>(yaml);
        REQUIRE(*result == original);
    }

    SECTION("round trip bool")
    {
        bool original = true;
        std::string yaml = prism::yaml::toYamlString(original);
        auto result = prism::yaml::fromYamlString<bool>(yaml);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismYaml - list container tests", "[yaml][container][list]")
{
    SECTION("serialize list<int>")
    {
        std::list<int> val = {11, 22, 33};
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("11") != std::string::npos);
        REQUIRE(yaml.find("22") != std::string::npos);
        REQUIRE(yaml.find("33") != std::string::npos);
    }

    SECTION("serialize list<string>")
    {
        std::list<std::string> val = {{"apple", "banana", "cherry"}};
        std::string yaml = prism::yaml::toYamlString(val);
        REQUIRE(yaml.find("apple") != std::string::npos);
        REQUIRE(yaml.find("banana") != std::string::npos);
        REQUIRE(yaml.find("cherry") != std::string::npos);
    }

    SECTION("deserialize list<int>")
    {
        std::string yaml = "[100, 200, 300]";
        auto result = prism::yaml::fromYamlString<std::list<int>>(yaml);
        REQUIRE(result->size() == 3);
        std::vector<int> v(result->begin(), result->end());
        REQUIRE(v[0] == 100);
        REQUIRE(v[1] == 200);
        REQUIRE(v[2] == 300);
    }

    SECTION("deserialize list<string>")
    {
        std::string yaml = "[\"a\", \"b\", \"c\"]";
        auto result = prism::yaml::fromYamlString<std::list<std::string>>(yaml);
        REQUIRE(result->size() == 3);
        std::vector<std::string> v(result->begin(), result->end());
        REQUIRE(v[0] == "a");
        REQUIRE(v[1] == "b");
        REQUIRE(v[2] == "c");
    }

    SECTION("serialize struct with list<int> (tst_struct::my_list_int)")
    {
        tst_struct obj;
        obj.my_list_int = {7, 8, 9};
        std::string yaml = prism::yaml::toYamlString(obj);
        REQUIRE(yaml.find("my_list_int:") != std::string::npos);
    }

    SECTION("deserialize struct with list<int>")
    {
        std::string yaml = "my_list_int: [1, 2, 3]";
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->my_list_int.size() == 3);
    }
}

TEST_CASE("prismYaml - datetime types", "[yaml][datetime]")
{
    SECTION("serialize datetime")
    {
        auto datetime = initDatetime(2023, 6, 23, 12, 30, 45);
        std::string yaml = prism::yaml::toYamlString(datetime);
        REQUIRE(yaml.find("2023") != std::string::npos);
        REQUIRE(yaml.find("06") != std::string::npos);
        REQUIRE(yaml.find("23") != std::string::npos);
    }

    SECTION("deserialize datetime from ISO 8601 format")
    {
        std::string yaml = "\"2023-06-23T12:30:45Z\"";
        auto result = prism::yaml::fromYamlString<std::chrono::system_clock::time_point>(yaml);
        std::time_t timestamp = std::chrono::system_clock::to_time_t(*result);
        std::tm* timeInfo = std::gmtime(&timestamp);
        REQUIRE(timeInfo->tm_year + 1900 == 2023);
        REQUIRE(timeInfo->tm_mon + 1 == 6);
        REQUIRE(timeInfo->tm_mday == 23);
    }

    SECTION("deserialize datetime from ISO 8601 format (verify year)")
    {
        std::string yaml = "\"2023-06-23T12:30:45Z\"";
        auto result = prism::yaml::fromYamlString<std::chrono::system_clock::time_point>(yaml);
        std::time_t timestamp = std::chrono::system_clock::to_time_t(*result);
        std::tm* timeInfo = std::gmtime(&timestamp);
        REQUIRE(timeInfo->tm_year + 1900 == 2023);
    }

    SECTION("serialize timestamp")
    {
        auto timestamp = initTimestamp(2023, 6, 23, 12, 30, 45);
        std::string yaml = prism::yaml::toYamlString(timestamp);
        REQUIRE(yaml.find("1687") != std::string::npos);
    }

    SECTION("deserialize timestamp")
    {
        std::time_t original = initTimestamp(2023, 6, 23, 12, 30, 45);
        std::string yaml = std::to_string(original);
        auto result = prism::yaml::fromYamlString<std::time_t>(yaml);
        REQUIRE(*result == original);
    }
}

TEST_CASE("prismYaml - struct serialization", "[yaml]")
{
    SECTION("serialize struct with all fields")
    {
        tst_sub_struct val;
        val.my_int = 123;
        val.my_bool = true;
        val.my_float = 1.5f;
        val.my_double = 2.5;
        val.my_string = "test";

        std::string yaml = prism::yaml::toYamlString(val);

        REQUIRE(yaml.find("my_int:") != std::string::npos);
        REQUIRE(yaml.find("my_bool:") != std::string::npos);
        REQUIRE(yaml.find("my_string:") != std::string::npos);
    }

    SECTION("deserialize struct from yaml")
    {
        std::string yaml = "my_int: 456\nmy_bool: false\nmy_string: \"deserialize\"";
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 456);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "deserialize");
    }
}
