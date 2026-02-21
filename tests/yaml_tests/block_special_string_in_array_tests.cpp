#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format struct array with special string values", "[yaml][block][array][string]")
{
    SECTION("vector of structs with colon-containing string values block round trip")
    {
        // Strings with colons must be quoted by the serializer
        std::vector<tst_sub_struct> original;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_string = "server: localhost:8080";  // contains colons
        original.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_string = "key: value pair";  // contains colon and space
        original.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<std::vector<tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].my_int == 10);
        REQUIRE((*result)[0].my_string == "server: localhost:8080");
        REQUIRE((*result)[1].my_int == 20);
        REQUIRE((*result)[1].my_string == "key: value pair");
    }

    SECTION("tst_struct my_vec_sp with hash-containing string block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 99;
        sub.my_string = "color: #FF0000";  // hash is YAML comment char
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_vec_sp[0].my_string == "color: #FF0000");
    }

    SECTION("block array with quoted boolean-looking strings round trip")
    {
        // Strings that look like booleans need quotes to avoid misinterpretation
        std::vector<tst_sub_struct> original;

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "true";  // boolean lookalike - must be quoted
        original.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "false";  // boolean lookalike - must be quoted
        original.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<std::vector<tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].my_string == "true");
        REQUIRE((*result)[1].my_string == "false");
    }
}
