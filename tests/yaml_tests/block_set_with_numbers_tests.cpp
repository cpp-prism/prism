#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_set_str with various string values round trip", "[yaml][block][set][string]")
{
    SECTION("my_set_str with number-like strings block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"123", "456", "789"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("123") == 1);
        REQUIRE(result->my_set_str.count("456") == 1);
        REQUIRE(result->my_set_str.count("789") == 1);
    }

    SECTION("my_set_str with unicode strings block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"中文", "日本語"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("中文") == 1);
        REQUIRE(result->my_set_str.count("日本語") == 1);
    }

    SECTION("my_set_str with null-like string block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"null_str"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 1);
        REQUIRE(result->my_set_str.count("null_str") == 1);
    }
}
