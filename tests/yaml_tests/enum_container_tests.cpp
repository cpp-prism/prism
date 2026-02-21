#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - enum container serialization", "[yaml][enum][container]")
{
    SECTION("vector<language> block round trip")
    {
        std::vector<language> original = {unknow, english, SimplifiedChinese, TraditionalChinese};
        std::string yaml = prism::yaml::toYamlStringBlock(original);
        REQUIRE(yaml.find("英语") != std::string::npos);

        auto result = prism::yaml::fromYamlString<std::vector<language>>(yaml);
        REQUIRE(result->size() == 4);
        REQUIRE((*result)[0] == unknow);
        REQUIRE((*result)[1] == english);
        REQUIRE((*result)[2] == SimplifiedChinese);
        REQUIRE((*result)[3] == TraditionalChinese);
    }

    SECTION("vector<language> flow round trip")
    {
        std::vector<language> original = {english, SimplifiedChinese};
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<language>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0] == english);
        REQUIRE((*result)[1] == SimplifiedChinese);
    }

    SECTION("map<string,language> block round trip")
    {
        std::map<std::string, language> original;
        original["a"] = english;
        original["b"] = SimplifiedChinese;
        original["c"] = unknow;

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, language>>(yaml);
        REQUIRE(result->size() == 3);
        REQUIRE((*result)["a"] == english);
        REQUIRE((*result)["b"] == SimplifiedChinese);
        REQUIRE((*result)["c"] == unknow);
    }

    SECTION("tst_struct my_vec_enum field round trip")
    {
        tst_struct original;
        original.my_vec_enum = {english, SimplifiedChinese};

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == english);
        REQUIRE(result->my_vec_enum[1] == SimplifiedChinese);
    }

    SECTION("tst_struct my_map_enum field round trip")
    {
        tst_struct original;
        original.my_map_enum["lang1"] = english;
        original.my_map_enum["lang2"] = TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["lang1"] == english);
        REQUIRE(result->my_map_enum["lang2"] == TraditionalChinese);
    }
}
