#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <unordered_map>

TEST_CASE("prismYaml - standalone unordered_map in block format", "[yaml][block][unordered_map]")
{
    SECTION("unordered_map<string, int> block round trip")
    {
        std::unordered_map<std::string, int> obj;
        obj["alpha"] = 1;
        obj["beta"] = 2;
        obj["gamma"] = 3;

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, int>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)["alpha"] == 1);
        REQUIRE((*result)["beta"] == 2);
        REQUIRE((*result)["gamma"] == 3);
    }

    SECTION("unordered_map<string, string> block round trip")
    {
        std::unordered_map<std::string, std::string> obj;
        obj["key1"] = "value1";
        obj["key2"] = "value2";

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, std::string>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["key1"] == "value1");
        REQUIRE((*result)["key2"] == "value2");
    }

    SECTION("unordered_map<string, tst_sub_struct> block round trip")
    {
        std::unordered_map<std::string, tst_sub_struct> obj;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        s1.my_string = "val_one";
        obj["one"] = s1;

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        s2.my_string = "val_two";
        obj["two"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["one"].my_int == 10);
        REQUIRE((*result)["one"].my_bool == true);
        REQUIRE((*result)["one"].my_string == "val_one");
        REQUIRE((*result)["two"].my_int == 20);
        REQUIRE((*result)["two"].my_bool == false);
        REQUIRE((*result)["two"].my_string == "val_two");
    }

    SECTION("empty unordered_map block round trip")
    {
        std::unordered_map<std::string, int> obj;

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::unordered_map<std::string, int>>(yaml);

        REQUIRE(result->empty());
    }
}
