#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - enum container serialization", "[json][enum][container]")
{
    SECTION("vector<language> serialization")
    {
        std::vector<language> langs = {english, SimplifiedChinese, TraditionalChinese};
        std::string json = prism::json::toJsonString(langs);
        REQUIRE(json.find("英语") != std::string::npos);
        REQUIRE(json.find("简体中文") != std::string::npos);
        REQUIRE(json.find("瀪体中文") != std::string::npos);
    }

    SECTION("vector<language> round trip")
    {
        std::vector<language> original = {unknow, english, SimplifiedChinese, TraditionalChinese};
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::vector<language>>(json);
        REQUIRE(result->size() == 4);
        REQUIRE((*result)[0] == unknow);
        REQUIRE((*result)[1] == english);
        REQUIRE((*result)[2] == SimplifiedChinese);
        REQUIRE((*result)[3] == TraditionalChinese);
    }

    SECTION("map<string,language> round trip")
    {
        std::map<std::string, language> original;
        original["a"] = english;
        original["b"] = SimplifiedChinese;
        original["c"] = unknow;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::map<std::string, language>>(json);
        REQUIRE(result->size() == 3);
        REQUIRE((*result)["a"] == english);
        REQUIRE((*result)["b"] == SimplifiedChinese);
        REQUIRE((*result)["c"] == unknow);
    }

    SECTION("tst_struct my_vec_enum field round trip")
    {
        tst_struct original;
        original.my_vec_enum = {english, SimplifiedChinese};

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == english);
        REQUIRE(result->my_vec_enum[1] == SimplifiedChinese);
    }

    SECTION("tst_struct my_map_enum field round trip")
    {
        tst_struct original;
        original.my_map_enum["lang1"] = english;
        original.my_map_enum["lang2"] = TraditionalChinese;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["lang1"] == english);
        REQUIRE(result->my_map_enum["lang2"] == TraditionalChinese);
    }
}
