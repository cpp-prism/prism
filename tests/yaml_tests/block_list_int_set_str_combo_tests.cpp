#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_int and my_set_str together round trip", "[yaml][block][list_int][set_str][combo]")
{
    SECTION("list_int and set_str both non-empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_int = {100, 200, 300};
        obj.my_set_str = {"a", "b", "c"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("b") == 1);
    }

    SECTION("list_int with negatives and set_str with multiple strings block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_int = {-10, -20, -30};
        obj.my_set_str = {"first", "second"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_list_int.front() == -10);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("first") == 1);
    }

    SECTION("list_int and set_str both empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_set_str.empty());
    }
}
