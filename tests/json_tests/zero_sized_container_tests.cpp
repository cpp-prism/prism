#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismJson - zero sized container", "[json][zero_size][container]")
{
    SECTION("serialize empty vector<int>")
    {
        std::vector<int> val = {};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "[]");
    }

    SECTION("serialize empty list<string>")
    {
        std::list<std::string> val = {};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "[]");
    }

    SECTION("serialize empty map<string, int>")
    {
        std::map<std::string, int> val = {};
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "{}");
    }

    SECTION("deserialize empty vector<int>")
    {
        std::string json = "[]";
        auto result = prism::json::fromJsonString<std::vector<int>>(json);
        REQUIRE(result->empty());
        REQUIRE(result->size() == 0);
    }

    SECTION("deserialize empty map<string, string>")
    {
        std::string json = "{}";
        auto result = prism::json::fromJsonString<std::map<std::string, std::string>>(json);
        REQUIRE(result->empty());
        REQUIRE(result->size() == 0);
    }

    SECTION("serialize struct with empty container")
    {
        tst_sub_struct obj;
        obj.my_vec_sp = {};
        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"my_vec_sp\":[]") != std::string::npos);
    }

    SECTION("deserialize struct with empty container")
    {
        std::string json = R"({"my_vec_sp":[]})";
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_vec_sp.size() == 0);
    }
}
