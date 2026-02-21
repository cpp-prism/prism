#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_list_int edge cases round trip", "[yaml][block][list][int]")
{
    SECTION("my_list_int with negative values block round trip")
    {
        tst_struct obj;
        obj.my_list_int = {-100, -1, 0, 1, 100};
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 5);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == -100);
        REQUIRE(*it++ == -1);
        REQUIRE(*it++ == 0);
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 100);
    }

    SECTION("my_list_std_string with empty string in middle block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"first", "", "third"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it++ == "first");
        REQUIRE(*it++ == "");
        REQUIRE(*it++ == "third");
    }

    SECTION("my_list_std_string with unicode block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"中文一", "中文二", "中文三"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it++ == "中文一");
        REQUIRE(*it++ == "中文二");
        REQUIRE(*it++ == "中文三");
    }

    SECTION("my_list_int with INT_MAX and INT_MIN block round trip")
    {
        tst_struct obj;
        obj.my_list_int = {INT_MAX, INT_MIN, 0};
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == INT_MAX);
        REQUIRE(*it++ == INT_MIN);
        REQUIRE(*it++ == 0);
    }
}
