#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format optional<string> with special YAML chars", "[yaml][block][optional][string][special]")
{
    SECTION("tst_sub_struct my_opt_str with colon-containing value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_opt_str = "key: value";  // colon must be handled

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "key: value");
    }

    SECTION("tst_sub_struct my_opt_str with hash-containing value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_opt_str = "color: #FF0000";  // hash is YAML comment char

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "color: #FF0000");
    }

    SECTION("tst_sub_struct my_opt_str with boolean-lookalike value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_opt_str = "true";  // looks like boolean but is a string

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "true");
    }

    SECTION("tst_sub_struct my_opt_str with null-looking value block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 4;
        obj.my_opt_str = "null";  // looks like null but is a string

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "null");
    }

    SECTION("map<string, tst_sub_struct> with Unicode key block round trip")
    {
        std::map<std::string, tst_sub_struct> obj;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_string = "alpha";
        obj["中文键"] = s1;

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::map<std::string, tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 1);
        REQUIRE(result->count("中文键") == 1);
        REQUIRE((*result)["中文键"].my_int == 10);
        REQUIRE((*result)["中文键"].my_string == "alpha");
    }
}
