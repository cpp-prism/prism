#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_std_string with various string values round trip", "[yaml][block][list][string][edge]")
{
    SECTION("my_list_std_string with various content types block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"hello", "world", "foo"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it == "hello"); ++it;
        REQUIRE(*it == "world"); ++it;
        REQUIRE(*it == "foo");
    }

    SECTION("my_list_std_string with unicode block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"中文", "한국어", "日本語"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it == "中文"); ++it;
        REQUIRE(*it == "한국어"); ++it;
        REQUIRE(*it == "日本語");
    }

    SECTION("my_list_std_string alongside my_list_int block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int = {5, 10, 15};
        obj.my_list_std_string = {"alpha", "beta", "gamma"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_list_std_string.size() == 3);
        auto sit = result->my_list_std_string.begin();
        REQUIRE(*sit == "alpha"); ++sit;
        REQUIRE(*sit == "beta"); ++sit;
        REQUIRE(*sit == "gamma");
    }
}
